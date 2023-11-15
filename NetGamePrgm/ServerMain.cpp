#include "Session.h"
#include "error.h"
#include <mutex>

std::array<Session, MAX_USER> g_players;

int g_ClientNum;
bool g_AllPlayerReady = false;
CRITICAL_SECTION g_cs;

DWORD WINAPI ClientThread(LPVOID socket);
DWORD WINAPI do_send(LPVOID);
DWORD WINAPI ProcessPacket(LPVOID socket);
///////////////////////////////////////////////////////////
int main()
{
	InitializeCriticalSection(&g_cs);
	int retval;	
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	//소켓 생성
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
	//로그인 
	while (!g_AllPlayerReady) {
		for (int i = 0; i < MAX_USER; ++i) {
			if (!g_players[i].GetAcceptPlayer()) {
				clientIndex = i;
				break;
			}
			else {	//이미 접속해있으면 로그인값 아이디 -1로 설정
				clientIndex = -1;
				break;
			}
		}

		addrlen = sizeof clientaddr;
		g_players[clientIndex].SetSocketInfo();
		g_players[clientIndex].SetSocket(accept(listen_socket, (struct sockaddr*)&clientaddr, &addrlen));
		if (g_players[clientIndex].GetSocket() == INVALID_SOCKET) {
			err_quit("accept()");
			std::cout << clientIndex << "번 클라이언트 오류 " << std::endl;
		}
		g_players[clientIndex].SetId(clientIndex);
		g_players[clientIndex].SetAcceptPlayer(true);
		g_ClientNum++;
		
		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)g_players[clientIndex].GetSocketInfo(), 0, 0);

		if (hThread == NULL) {
			closesocket(g_players[clientIndex].GetSocket());
		}
		else {
			CloseHandle(hThread);
		}
		if (g_ClientNum == 2) break;
	}
	DeleteCriticalSection(&g_cs);

	closesocket(listen_socket);

	WSACleanup();
	return 0;
}
//클라이언트 스레드 - 접속한 클라 정보 가지고 있고, 데이터를 받아서 작업자 스레드에 넘김
DWORD WINAPI ClientThread(LPVOID socket)
{
	int retval;
	HANDLE sThread;
	HANDLE rThread;

	SocketInfo* socketinfo = reinterpret_cast<SocketInfo*>(socket);
	

	std::cout << "id:" << socketinfo->id << std::endl;

	SOCKET client_socket = socketinfo->client_socket;

	while (1) {
		retval = recv(client_socket, socketinfo->buf, sizeof socketinfo->buf, 0);
		std::cout << socketinfo->buf << std::endl;
	
		rThread = CreateThread(NULL, 0, ProcessPacket, (LPVOID)socket, 0, NULL);
		if (rThread == NULL) { closesocket(client_socket); }
		else { CloseHandle(rThread); }

		sThread = CreateThread(NULL, 0, do_send, NULL, 0, NULL);
		if (sThread == NULL) { closesocket(client_socket); }
		else { CloseHandle(sThread); }
	}

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

//클라로부터 받은 데이터 처리
DWORD WINAPI ProcessPacket(LPVOID socket)
{
	SocketInfo* socketinfo = reinterpret_cast<SocketInfo*>(socket);
	SOCKET client_socket = socketinfo->client_socket;

	while (true) {
		char* p = reinterpret_cast<char*>(socketinfo->buf);
		switch (p[0]) {
		case CS_LOGIN: {
			CS_LOGIN_PACKET* cspacket = reinterpret_cast<CS_LOGIN_PACKET*>(p);
			g_players[socketinfo->id].SetName(cspacket->name);
			g_players[socketinfo->id].SetOnline(true);

			//자신 포함 다른클라에게도 로그인했다고 보내기

			SC_LOGIN_OK_PACKET* scpacket = new SC_LOGIN_OK_PACKET;
			scpacket->type = SC_LOGIN_OK;

			for (int i = 0; i < MAX_USER; ++i) {
				if (g_players[i].GetOnline()) {
					EnterCriticalSection(&g_cs);
					send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), sizeof(SC_LOGIN_OK_PACKET), 0);
					LeaveCriticalSection(&g_cs);
				}
			}
			delete scpacket;
		}
			break;
		case CS_READY: {
			CS_READY_PACKET* cspacket = reinterpret_cast<CS_READY_PACKET*>(p);
			g_players[socketinfo->id].SetReadyPlayer(true);

			//자신 포함 다른클라에게도 레디했다고 보내기
			SC_READY_PACKET* scpacket = new SC_READY_PACKET;
			scpacket->type = SC_READY_OK;

			for (int i = 0; i < MAX_USER; ++i) {
				if (g_players[i].GetOnline()) {
					EnterCriticalSection(&g_cs);
					send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), sizeof(SC_READY_PACKET), 0);
					LeaveCriticalSection(&g_cs);
				}
			}
			delete scpacket;
		}
			break;
		}
	}
	return 0;
}
