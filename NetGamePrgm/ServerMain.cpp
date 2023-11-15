#include "Session.h"
#include "error.h"
#include <mutex>
#include <windows.h>

std::array<Session, MAX_USER> g_players;

int g_ClientNum;
bool g_AllPlayerReady = false;
CRITICAL_SECTION g_cs;
std::mutex g_mutex;

DWORD WINAPI ClientThread(LPVOID socket);
DWORD WINAPI do_send(LPVOID);
///////////////////////////////////////////////////////////
int main()
{
	InitializeCriticalSection(&g_cs);
	int retval;	
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	//���� ����
	SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == INVALID_SOCKET) err_quit("socket()");
	
	//bind()	
	struct sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	
	//listen()
	retval = listen(listen_socket, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	
	struct sockaddr_in clientaddr;
	int addrlen{};
	HANDLE hThread;
	
	int clientIndex{};
	//�α��� 
	while (!g_AllPlayerReady) {
		for (int i = 0; i < MAX_USER; ++i) {
			if (!g_players[i].GetAcceptPlayer()) {
				clientIndex = i;
				break;
			}
		}

		addrlen = sizeof clientaddr;
		g_players[clientIndex].SetSocket(accept(listen_socket, (struct sockaddr*)&clientaddr, &addrlen));
		if (g_players[clientIndex].GetSocket() == INVALID_SOCKET) {
			err_quit("accept()");
			std::cout << clientIndex << "�� Ŭ���̾�Ʈ ���� " << std::endl;
		}
		g_players[clientIndex].SetId(clientIndex);
		g_players[clientIndex].SetAcceptPlayer(true);
		g_ClientNum++;
		
		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)g_players[clientIndex].GetSocketInfo(), 0, 0);
		std::cout << g_players[clientIndex].GetSocketInfo() << std::endl;

		if (hThread == NULL) {
			std::cout << "closesocket()" << std::endl;
			closesocket(g_players[clientIndex].GetSocket());
		}
		else {
			std::cout << "closeHandle()" << std::endl;
			CloseHandle(hThread);
		}
		if (g_ClientNum == 2) break;
	}

	while (true) {
		std::cout << "��� �α����� �Ϸ�ƾ�!!" << std::endl;
		Sleep(50000);
	}
	DeleteCriticalSection(&g_cs);

	closesocket(listen_socket);

	WSACleanup();
	return 0;
}

DWORD WINAPI do_send(LPVOID)
{
	while (true) {
		for (int i = 0; i < MAX_USER; ++i) {
			if (g_players[i].GetOnline()) {
				SC_LOGIN_INFO_PACKET* scpacket = new SC_LOGIN_INFO_PACKET;
				scpacket->type = SC_LOGIN_INFO;
				scpacket->x = X;
				scpacket->y = Y;
				scpacket->hp = HP;
				scpacket->speed = SPEED;
				scpacket->bullet_cnt = BULLET_CNT;

				g_players[i].SetX(scpacket->x);
				g_players[i].SetY(scpacket->y);
				g_players[i].SetHp(scpacket->hp);
				g_players[i].SetSpeed(scpacket->speed);
				g_players[i].setBulletCnt(scpacket->bullet_cnt);

				EnterCriticalSection(&g_cs);
				send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), sizeof SC_LOGIN_INFO_PACKET, 0);
				LeaveCriticalSection(&g_cs);

				delete scpacket;
			}
		}
	}
}

DWORD WINAPI ClientThread(LPVOID socket)
{
	int retval;

	SocketInfo* socketinfo = reinterpret_cast<SocketInfo*>(socket);
	SOCKET client_socket = socketinfo->client_socket;

	int len;
	char buf[BUFSIZE];

	while (true) {
		retval = recv(client_socket, (char*)(&len), sizeof(int), MSG_WAITALL);
		retval = recv(client_socket, buf, len, MSG_WAITALL);
		
		char* p = reinterpret_cast<char*>(buf);
		switch (p[0]) {
		case CS_LOGIN: {
			CS_LOGIN_PACKET* cspacket = reinterpret_cast<CS_LOGIN_PACKET*>(p);
			g_players[socketinfo->id].SetName(cspacket->name);
			g_players[socketinfo->id].SetOnline(true);

			std::cout << "Ŭ��κ��� ���� ���̵� : " << g_players[socketinfo->id].GetName() << std::endl;
			//�ڽ� ���� �ٸ�Ŭ�󿡰Ե� �α����ߴٰ� ������

			SC_LOGIN_OK_PACKET* scpacket = new SC_LOGIN_OK_PACKET;
			scpacket->type = SC_LOGIN_OK;
			memcpy(scpacket->name, g_players[socketinfo->id].GetName(), sizeof(g_players[socketinfo->id].GetName()));
			len = sizeof(SC_LOGIN_OK_PACKET);

			{
				std::lock_guard<std::mutex> lock(g_mutex);
				for (int i = 0; i < MAX_USER; ++i) {
					if (g_players[i].GetOnline()) {
						/*EnterCriticalSection(&g_cs);*/
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), len, 0);
						/*LeaveCriticalSection(&g_cs);*/
					}
				}
			}
			delete scpacket;
		}
			break;
		case CS_READY: {
			CS_READY_PACKET* cspacket = reinterpret_cast<CS_READY_PACKET*>(p);
			g_players[socketinfo->id].SetReadyPlayer(true);

			//�ڽ� ���� �ٸ�Ŭ�󿡰Ե� �����ߴٰ� ������
			SC_READY_OK_PACKET* scpacket = new SC_READY_OK_PACKET;
			scpacket->type = SC_READY_OK;
			memcpy(scpacket->name, g_players[socketinfo->id].GetName(), sizeof(g_players[socketinfo->id].GetName()));
			len = sizeof(SC_READY_OK_PACKET);

			for (int i = 0; i < MAX_USER; ++i) {
				if (g_players[i].GetOnline()) {
					EnterCriticalSection(&g_cs);
					send(g_players[i].GetSocket(), (char*)&len, sizeof(int), 0);
					send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), len, 0);
					LeaveCriticalSection(&g_cs);
				}
			}
			
			delete scpacket;
		}
			break;
		}
		//����,���� �ʱ�ȭ
		memset(buf, 0, sizeof(buf));
		len = 0;
	}
	return 0;
}
