#include "Session.h"
#include "error.h"
//#include "Collision.h"
#include <mutex>
#include <windows.h>
#include "chrono"
#include <random>

static std::random_device rd;
static std::default_random_engine dre(rd());
static std::uniform_int_distribution<int> uid(-28, 28);
struct COLOR
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
};
//struct Map_Block
//{
//	bool exist = false;
//	float height = 0;
//
//	COLOR b_color;
//
//	//좌표
//	float x = 0.0;
//	float y = 0.0;
//	float z = 0.0;
//
//struct COLOR
//{
//	float r = 1.0f;
//	float g = 1.0f;
//	float b = 1.0f;
//};

struct Map_Block
{
	bool exist = false;
	float height = 0;

	COLOR b_color;

	//좌표
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	//크기
	float x_scale = 1.0;
	float y_scale = 2.0;
	float z_scale = 1.0;

}Block[30][30];

struct Heart {
	bool exist = true;
	float x = 3.0;
	float z = 0.0;
};

struct Wheel {
	bool exist = true;
	float x = 5.0;
	float z = 0.0;
};

struct ICE {
	bool exist = true;
	float x = 9.0;
	float z = 0.0;
};

struct	Heart heart[3];
struct	Wheel wheel[3];
struct 	ICE ice[3];

struct Sphere {
	bool launch = false;
	float sphere_z = 0;
	float now_yaw = 0;
	float x;
	float z;
	bool isfreeze;
};

struct Sphere sphere[BULLET_CNT]; // 포탄의 갯수 

int temp_x;
int temp_z;

//==맵 블럭 초기화========================================================================================

void Setup_Block() {


	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			Block[i][j].b_color.r = 0.2;
			Block[i][j].b_color.g = 0.1;
			Block[i][j].b_color.b = 0.0;


			Block[i][j].x = -29 + j * 2;
			Block[i][j].z = -29 + i * 2;
		}
	}


	// 테두리 벽

	for (int i = 0; i < 30; i++)
	{
		Block[0][i].exist = true;
		Block[i][0].exist = true;

		Block[29][i].exist = true;
		Block[i][29].exist = true;
	}
}

void make_map() {

	for (int i = 0; i < 3; i++)
	{
		//작은 꺽쇄
		Block[8][8 + i].exist = true;
		Block[8 + i][8].exist = true;

		Block[20][8 + i].exist = true;
		Block[20 - i][8].exist = true;


		Block[8][20 - i].exist = true;
		Block[8 + i][20].exist = true;


		Block[20][20 - i].exist = true;
		Block[20 - i][20].exist = true;

	}


	for (int i = 0; i < 2; i++)
	{
		//중앙 사각형
		//Block[13][14 + i].exist = true;
		//Block[16][14 + i].exist = true;
		//
		//Block[14+i][13].exist = true;
		//Block[14+i][16].exist = true;
	}



	for (int i = 0; i < 4; i++)
	{
		//큰 꺽쇄
		Block[4][4 + i].exist = true;
		Block[4 + i][4].exist = true;

		Block[25][4 + i].exist = true;
		Block[25 - i][4].exist = true;


		Block[4][25 - i].exist = true;
		Block[4 + i][25].exist = true;


		Block[25][25 - i].exist = true;
		Block[25 - i][25].exist = true;
	}

}

void item_setup()
{
	//for (int i = 0; i < 3; i++)
	//{	
	//		temp_x = uid(dre);
	//		temp_z = uid(dre);

	//		heart[i].x = temp_x;
	//		heart[i].z = temp_z;
	//	//-------------------------

	//		temp_x = uid(dre);
	//		temp_z = uid(dre);

	//		wheel[i].x = temp_x;
	//		wheel[i].z = temp_z;
	//	//-------------------------
	//		temp_x = uid(dre);
	//		temp_z = uid(dre);

	//		ice[i].x = temp_x;
	//		ice[i].z = temp_z;
	//		
	//	
	//}
	heart[0].x = 15;
	heart[0].z = 10;

	heart[1].x = -15;
	heart[1].z = -10;

	heart[2].x = 0;
	heart[2].z = 12;

	wheel[0].x = -15;
	wheel[0].z = 10;

	wheel[1].x = 15;
	wheel[1].z = -10;

	wheel[2].x = 0;
	wheel[2].z = -12;

	ice[0].x = 20;
	ice[0].z = 0;

	ice[1].x = -20;
	ice[1].z = 0;

	ice[2].x = 0;
	ice[2].z = 0;


	/*heart[1].x = 10;
	heart[1].z = 10;

	heart[2].x = 15;
	heart[2].z = 15;*/
}

std::array<Session, MAX_USER> g_players;
int g_bullet_num;
int g_ClientNum;
bool g_AllPlayerReady = false;
bool g_GameStart = false;
bool g_GameEnd = false;
CRITICAL_SECTION g_maincs;
CRITICAL_SECTION g_clientThreadcs;
std::mutex g_Recvmutex;
std::mutex g_Sendmutex;

DWORD WINAPI ClientThread(LPVOID socket);
DWORD WINAPI do_send(LPVOID lpParam);

void tank_collid(std::array<Session, MAX_USER>& players);
void wall_collid(std::array<Session, MAX_USER>& players, short id);
bool collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB);

///////////////////////////////////////////////////////////
int main()
{

	Setup_Block();
	make_map();
	//맵생성
	Setup_Block();
	make_map();
	InitializeCriticalSection(&g_maincs);
	InitializeCriticalSection(&g_clientThreadcs);
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

	HANDLE SendThread;
	SendThread = CreateThread(NULL, 0, do_send, 0, 0, 0);
	if (SendThread == NULL) {
		std::cout << "closesocket()" << std::endl;
		closesocket(listen_socket);
	}
	else {
		CloseHandle(SendThread);
	}

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
		}

		addrlen = sizeof clientaddr;
		g_players[clientIndex].SetSocket(accept(listen_socket, (struct sockaddr*)&clientaddr, &addrlen));
		if (g_players[clientIndex].GetSocket() == INVALID_SOCKET) {
			err_quit("accept()");
			//std::cout << clientIndex << "번 클라이언트 오류 " << std::endl;
		}
		// TCP_NODELAY 옵션 활성화
		int flag = 1;
		setsockopt(g_players[clientIndex].GetSocket(), IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

		g_players[clientIndex].SetId(clientIndex);
		g_players[clientIndex].SetAcceptPlayer(true);
		g_players[clientIndex].SetX(10 * clientIndex);	//이부분 백하빈 코드
		g_ClientNum++;

		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)g_players[clientIndex].GetSocketInfo(), 0, 0);
		//std::cout << g_players[clientIndex].GetSocketInfo() << std::endl;

		if (hThread == NULL) {
			//std::cout << "closesocket()" << std::endl;
			closesocket(g_players[clientIndex].GetSocket());
		}
		else {
			//std::cout << "closeHandle()" << std::endl;
			CloseHandle(hThread);
		}
		if (g_ClientNum == 2) {
			break;
		}

	}
	//인 게임
	/////////////////////////////////////////////////////////////////////////////////////////
	//1.아이템 생성
	item_setup();
	/////////////////////////////////////////////////////////////////////////////////////////
	//HEAL
	{
		SC_SET_ITEM_PACKET* packet = new SC_SET_ITEM_PACKET;
		packet->type = SC_SET_ITEM;
		int len = sizeof SC_SET_ITEM_PACKET;
		packet->item_type = HEAL;
		for (int i = 0; i < 3; ++i) {
			packet->exist[i] = true;
			packet->x[i] = heart[i].x;
			packet->z[i] = heart[i].z;
		}
		for (int i = 0; i < MAX_USER; ++i) {
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
		}
		std::cout << "힐패킷 보내기" << std::endl;
		delete packet;
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//SPEEDUP
	{
		SC_SET_ITEM_PACKET* packet = new SC_SET_ITEM_PACKET;
		packet->type = SC_SET_ITEM;
		int len = sizeof SC_SET_ITEM_PACKET;
		packet->item_type = SPEEDUP;
		for (int i = 0; i < 3; ++i) {
			packet->exist[i] = true;
			packet->x[i] = wheel[i].x;
			packet->z[i] = wheel[i].z;
		}
		for (int i = 0; i < MAX_USER; ++i) {
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
		}
		std::cout << "스피업패킷 보내기" << std::endl;
		delete packet;
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//FREEZE
	{
		SC_SET_ITEM_PACKET* packet = new SC_SET_ITEM_PACKET;
		packet->type = SC_SET_ITEM;
		int len = sizeof SC_SET_ITEM_PACKET;
		packet->item_type = FREEZE;
		for (int i = 0; i < 3; ++i) {
			packet->exist[i] = true;
			packet->x[i] = ice[i].x;
			packet->z[i] = ice[i].z;
		}
		for (int i = 0; i < MAX_USER; ++i) {
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
		}
		std::cout << "얼음패킷 보내기" << std::endl;
		delete packet;
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	//게임 전

	while (true) {
		int readyClient = 0;

		if (g_GameStart) break;

		for (int i = 0; i < MAX_USER; ++i) {
			if (g_players[i].GetReadyPlayer()) {
				readyClient++;
			}
		}
		if (readyClient == MAX_USER) {
			SC_GAMESTART_PACKET* packet = new SC_GAMESTART_PACKET;
			packet->tpye = SC_GAMESTART;
			int len = sizeof SC_GAMESTART_PACKET;
			for (int i = 0; i < MAX_USER; ++i) {
				EnterCriticalSection(&g_maincs);
				send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(g_players[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
				LeaveCriticalSection(&g_maincs);
			}
			EnterCriticalSection(&g_maincs);
			g_GameStart = true;
			LeaveCriticalSection(&g_maincs);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//게임 시작 후 - 아이템 세팅
	{
		SC_ALL_ITEM_SET_PACKET* packet = new SC_ALL_ITEM_SET_PACKET;
		packet->type = SC_ALL_ITEM_SET;
		int len = sizeof SC_ALL_ITEM_SET_PACKET;
		for (int i = 0; i < MAX_USER; ++i) {
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
			send(g_players[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
		}
		delete packet;
		//std::cout << "아이템 생성" << std::endl;
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	std::cout << "게임 종료 루프 전 오니?" << std::endl;
	//게임 종료
	while (true) {
		if (g_GameEnd) {
			std::cout << "게임종료" << std::endl;
			closesocket(listen_socket);
			WSACleanup();
			break;
		}
	}
	DeleteCriticalSection(&g_maincs);
	DeleteCriticalSection(&g_clientThreadcs);
	return 0;
}

// 초당패킷을 계산하여 클라에게 계속 뿌려준다
DWORD WINAPI do_send(LPVOID lpParam)
{
	int len;
	char buf[BUFSIZE];

	SC_UPDATE_PACKET* scpacket = new SC_UPDATE_PACKET;
	len = sizeof(SC_UPDATE_PACKET);
	scpacket->type = SC_UPDATE;

	std::chrono::steady_clock::time_point last_send_time = std::chrono::steady_clock::now();
	while (true) {
		//if (g_GameEnd) return 0;
		if (!g_GameStart) continue;
		auto current_time = std::chrono::steady_clock::now();
		auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_send_time).count();

		if (elapsed_time >= 1000 / 33) {
			for (int i = 0; i < MAX_USER; ++i) {
				if (g_players[i].GetOnline()) {
					scpacket->id = g_players[i].GetId();
					scpacket->hp = g_players[i].GetHp();
					scpacket->x = g_players[i].GetX();
					scpacket->y = g_players[i].GetY();
					scpacket->z = g_players[i].GetZ();
					scpacket->result = g_players[i].GetResult();
					scpacket->speed = g_players[i].GetSpeed();
					scpacket->bullet_cnt = g_players[i].GetBulletCnt();
					scpacket->yaw = g_players[i].GetYaw();
					scpacket->bodyYaw = g_players[i].GetBodyYaw();

				}
				for (int i = 0; i < MAX_USER; ++i) {
					std::lock_guard<std::mutex> lock(g_Sendmutex);
					if (g_players[i].GetOnline()) {
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), len, 0);
					}
				}
			}
			last_send_time = current_time;
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
			std::cout << "접속 클라 아이디 : " << socketinfo->id << std::endl;
			CS_LOGIN_PACKET* cspacket = reinterpret_cast<CS_LOGIN_PACKET*>(p);
			g_players[socketinfo->id].SetName(cspacket->name);
			g_players[socketinfo->id].SetOnline(true);
			int namelen = strlen(cspacket->name);
			//std::cout << "클라로부터 얻어온 아이디 : " << g_players[socketinfo->id].GetName() << std::endl;
			
			//g_player에 초기 정보 받아오기SC_UPDATE_PACKET
			g_players[socketinfo->id].SetHp(HP);
			g_players[socketinfo->id].setBulletCnt(BULLET_CNT);
			g_players[socketinfo->id].SetSpeed(SPEED);
			//g_players[socketinfo->id].SetX(X);
			//g_players[socketinfo->id].SetY(Y);
			//g_players[socketinfo->id].SetY(Z);
			
			//자신 포함 다른클라에게도 로그인했다고 보내기
			SC_LOGIN_OK_PACKET* scpacket = new SC_LOGIN_OK_PACKET;
			scpacket->type = SC_LOGIN_OK;
			memcpy(scpacket->name, g_players[socketinfo->id].GetName(), sizeof(g_players[socketinfo->id].GetName()));
			scpacket->id = g_players[socketinfo->id].GetId();
			scpacket->hp = g_players[socketinfo->id].GetHp();
			scpacket->bullet_cnt = g_players[socketinfo->id].GetBulletCnt();
			//scpacket->x = g_players[socketinfo->id].GetX();
			//scpacket->y = g_players[socketinfo->id].GetY();
			//scpacket->z = g_players[socketinfo->id].GetZ();
			scpacket->speed = g_players[socketinfo->id].GetSpeed();
			std::cout << "서버가 보내는 아이디 : " << scpacket->id << std::endl;
			len = sizeof(SC_LOGIN_OK_PACKET);

			{
				std::lock_guard<std::mutex> lock(g_Recvmutex);
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
			EnterCriticalSection(&g_clientThreadcs);
			if (!g_players[socketinfo->id].GetReadyPlayer()) {
				g_players[socketinfo->id].SetReadyPlayer(true);
				std::cout << "몇번 클라가 레디 했나 - " << socketinfo->id << std::endl;
			}
			else {
				g_players[socketinfo->id].SetReadyPlayer(false);
				std::cout << "몇번 클라가 언레디 했나 - " << socketinfo->id << std::endl;
			}
			LeaveCriticalSection(&g_clientThreadcs);

			//자신 포함 다른클라에게도 레디했다고 보내기
			SC_READY_OK_PACKET* scpacket = new SC_READY_OK_PACKET;
			scpacket->type = SC_READY_OK;
			memcpy(scpacket->name, g_players[socketinfo->id].GetName(), sizeof(g_players[socketinfo->id].GetName()));
			//scpacket->id = g_players[socketinfo->id].GetId();
			len = sizeof(SC_READY_OK_PACKET);

			{
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				for (int i = 0; i < MAX_USER; ++i) {
					if (g_players[i].GetOnline()) {
						//EnterCriticalSection(&g_cs);
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), len, 0);
						//LeaveCriticalSection(&g_cs);
					}
				}
			}
			delete scpacket;
		}
					 break;
		case CS_MOVE: {
			CS_MOVE_PACKET* cspacket = reinterpret_cast<CS_MOVE_PACKET*>(p);
			switch (cspacket->direction) {
			case DIRECTION::UP: {
				EnterCriticalSection(&g_clientThreadcs);
				float tempx = g_players[socketinfo->id].GetSpeed() * sin(cspacket->bodyYaw * 3.14 / 180.0f);
				float tempz = g_players[socketinfo->id].GetSpeed() * cos(cspacket->bodyYaw * 3.14 / 180.0f);

				std::cout << cspacket->bodyYaw << std::endl;

				g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() - tempx);
				g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() - tempz);


				//std::cout << socketinfo->id << "번 클라 w누를때 : " << g_players[socketinfo->id].GetZ() + 0.3 << std::endl;
				tank_collid(g_players);
				wall_collid(g_players, socketinfo->id);
				if (g_players[socketinfo->id].GetCollision())
				{
					std::cout << "탱크끼리 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() + tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() + tempz);
					g_players[0].SetCollision(false);
					g_players[1].SetCollision(false);
				}
				if (g_players[socketinfo->id].GetWallCollision())
				{
					std::cout << "탱크-벽 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() + tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() + tempz);
					g_players[socketinfo->id].SetWallCollision(false);

				}
				LeaveCriticalSection(&g_clientThreadcs);
			}
							  break;
			case DIRECTION::DOWN: {
				EnterCriticalSection(&g_clientThreadcs);
				float tempx = g_players[socketinfo->id].GetSpeed() * sin(cspacket->bodyYaw * 3.14 / 180.0f);
				float tempz = g_players[socketinfo->id].GetSpeed() * cos(cspacket->bodyYaw * 3.14 / 180.0f);

				std::cout << cspacket->bodyYaw << std::endl;

				g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() + tempx);
				g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() + tempz);
				//std::cout << socketinfo->id << "번 클라 s누를때 : " << g_players[socketinfo->id].GetZ() - 0.3 << std::endl;
				tank_collid(g_players);
				
				(g_players, socketinfo->id);
				if (g_players[socketinfo->id].GetCollision())
				{
					std::cout << "탱크끼리 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() - tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() - tempz);
					g_players[0].SetCollision(false);
					g_players[1].SetCollision(false);
				}
				if (g_players[socketinfo->id].GetWallCollision())
				{
					std::cout << "탱크-벽 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() - tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() - tempz);
					g_players[socketinfo->id].SetWallCollision(false);

				}
				LeaveCriticalSection(&g_clientThreadcs);
			}
								break;
			case DIRECTION::LEFT: {	
				EnterCriticalSection(&g_clientThreadcs);
				float tempx = g_players[socketinfo->id].GetSpeed() * sin(cspacket->bodyYaw * 3.14 / 180.0f + 3.14 / 2.0f);
				float tempz = g_players[socketinfo->id].GetSpeed() * cos(cspacket->bodyYaw * 3.14 / 180.0f + 3.14 / 2.0f);

				std::cout << cspacket->bodyYaw << std::endl;

				g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() - tempx);
				g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() - tempz);

				//std::cout << socketinfo->id << "번 클라 a누를때 : " << g_players[socketinfo->id].GetX() + 0.3 << std::endl;
				tank_collid(g_players);
				wall_collid(g_players, socketinfo->id);
				if (g_players[socketinfo->id].GetCollision())
				{
					std::cout << "탱크끼리 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() + tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() + tempz);
					g_players[0].SetCollision(false);
					g_players[1].SetCollision(false);
				}
				if (g_players[socketinfo->id].GetWallCollision())
				{
					std::cout << "탱크-벽 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() + tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() + tempz);
					g_players[socketinfo->id].SetWallCollision(false);

				}
				LeaveCriticalSection(&g_clientThreadcs);
			}
				break;
			case DIRECTION::RIGHT: {
				EnterCriticalSection(&g_clientThreadcs);
				float tempx = g_players[socketinfo->id].GetSpeed() * sin(cspacket->bodyYaw * 3.14 / 180.0f + 3.14 / 2.0f);
				float tempz = g_players[socketinfo->id].GetSpeed() * cos(cspacket->bodyYaw * 3.14 / 180.0f + 3.14 / 2.0f);

				std::cout << cspacket->bodyYaw << std::endl;

				g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() + tempx);
				g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() + tempz);
				//std::cout << socketinfo->id << "번 클라 d누를때 : " << g_players[socketinfo->id].GetX() - 0.3 << std::endl;
				tank_collid(g_players);
				wall_collid(g_players, socketinfo->id);
				if (g_players[socketinfo->id].GetCollision())
				{
					std::cout << "탱크끼리 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() - tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() - tempz);
					g_players[0].SetCollision(false);
					g_players[1].SetCollision(false);
				}
				if (g_players[socketinfo->id].GetWallCollision())
				{
					std::cout << "탱크-벽 충돌" << std::endl;
					g_players[socketinfo->id].SetX(g_players[socketinfo->id].GetX() - tempx);
					g_players[socketinfo->id].SetZ(g_players[socketinfo->id].GetZ() - tempz);
					g_players[socketinfo->id].SetWallCollision(false);

				}
				LeaveCriticalSection(&g_clientThreadcs);
			}
				break;
			}
		}
					break;
		case CS_YAW: {
			CS_YAW_PACKET* cspacket = reinterpret_cast<CS_YAW_PACKET*>(p);
			g_players[socketinfo->id].setYaw(cspacket->yaw);
			g_players[socketinfo->id].setBodyYaw(cspacket->bodyYaw);

			//std::cout << "탱크 대가리 돈거 데이터 오냐?" << std::endl;
			//std::cout << cspacket->yaw << std::endl;
		}
				   break;
		case CS_ATTACK: {
			CS_ATTACK_PACKET* cspacket = reinterpret_cast<CS_ATTACK_PACKET*>(p);
			////////////////////////////////////////////////////////////////////
			//이 구간은 클라로부터 받은 총알의 시작지점과 y값 발사bool값을 나타냄
			{
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				sphere[g_bullet_num].now_yaw = cspacket->now_yaw;
				sphere[g_bullet_num].x = cspacket->x;
				sphere[g_bullet_num].z = cspacket->z;
				sphere[g_bullet_num].launch = cspacket->isshoot;
			}

			//std::cout << cspacket->now_yaw << std::endl;
			//std::cout << cspacket->x << std::endl;
			//std::cout << cspacket->z << std::endl;

			////////////////////////////////////////////////////////////////////
			g_players[socketinfo->id].setBulletCnt(g_players[socketinfo->id].GetBulletCnt() - 1);	//한발씩 감소
			//std::cout << socketinfo->id << "번 클라이언트 포탄 발사!! 남은 총알 : " << g_players[socketinfo->id].GetBulletCnt() << std::endl;

			SC_ATTACK_PACKET* scpacket = new SC_ATTACK_PACKET;
			scpacket->type = SC_ATTACK;
			scpacket->id = socketinfo->id;
			scpacket->now_yaw = cspacket->now_yaw;
			scpacket->x = cspacket->x;
			scpacket->z = cspacket->z;
			scpacket->isshoot = true;
			scpacket->isfreeze = sphere[g_bullet_num].isfreeze;

			//std::cout << scpacket->id << std::endl;
			//std::cout << scpacket->now_yaw << std::endl;
			//std::cout << scpacket->x << std::endl;
			//std::cout << scpacket->z << std::endl;

			int len = sizeof SC_ATTACK_PACKET;
			{
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				for (int i = 0; i < MAX_USER; ++i) {
					if (g_players[i].GetOnline()) {
						//EnterCriticalSection(&g_cs);
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
						send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), len, 0);
						//LeaveCriticalSection(&g_cs);
					}
				}
			}
			++g_bullet_num;
			delete scpacket;
		}
					  break;
		case CS_ITEM: {
			CS_ITEM_PACKET* cspacket = reinterpret_cast<CS_ITEM_PACKET*>(p);
			//아이템 먹으면 아이템 컨테이너 상태 적용 시키고 g_player도 상태변화 시켜야한다.
			if (cspacket->item == HEAL) {	//힐패킷 빨간색 아이템
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				heart[cspacket->num].exist = false;
				g_players[socketinfo->id].SetHp(g_players[socketinfo->id].GetHp() + 20);
				std::cout << "힐팩 먹음" << std::endl;
			}
			if (cspacket->item == SPEEDUP) {	//스피드패킷 파란색 아이템
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				wheel[cspacket->num].exist = false;
				g_players[socketinfo->id].SetSpeed(g_players[socketinfo->id].GetSpeed() + 0.1);
				std::cout << "스피드팩 먹음" << std::endl;
			}
			if (cspacket->item == FREEZE) {		//얼음패킷 노란색 아이템
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				ice[cspacket->num].exist = false;
				sphere[g_bullet_num].isfreeze = true;
				std::cout << "프리즈팩 먹음" << std::endl;
			}
			
			SC_SET_ITEM_PACKET* packet = new SC_SET_ITEM_PACKET;
			packet->type = SC_SET_ITEM;
			int len = sizeof SC_SET_ITEM_PACKET;
			packet->item_type = cspacket->item;

			for (int i = 0; i < 3; i++)
			{
				if (packet->item_type == HEAL)
				{
					packet->exist[i] = heart[i].exist;
					packet->x[i] = heart[i].x;
					packet->z[i] = heart[i].z;
				}
				else if (packet->item_type == SPEEDUP)
				{
					packet->exist[i] = wheel[i].exist;
					packet->x[i] = wheel[i].x;
					packet->z[i] = wheel[i].z;
				}
				else if (packet->item_type == FREEZE)
				{
					packet->exist[i] = ice[i].exist;
					packet->x[i] = ice[i].x;
					packet->z[i] = ice[i].z;
				}
			}

			for (int i = 0; i < MAX_USER; i++)
			{
				send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(g_players[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
			}
		//	std::cout << "아이템 변경 값 보내줌" << std::endl;
		}
					break;
		case CS_RELOAD: {
			std::cout << socketinfo->id << "번째 클라 재장전" << std::endl;
			{
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				for (int i = 0; i < BULLET_CNT; ++i) {
					sphere[i].launch = false;
					sphere[i].now_yaw = 0;
					sphere[i].x = 0;
					sphere[i].z = 0;
					sphere[i].isfreeze = false;
					g_bullet_num = 0;
				}
				g_players[socketinfo->id].setBulletCnt(BULLET_CNT);
			}

			SC_RELOAD_PACKET* scpacket = new SC_RELOAD_PACKET;
			scpacket->type = SC_RELOAD;
			scpacket->bullet_num = g_bullet_num;
			int len = sizeof SC_RELOAD_PACKET;

			{
				std::lock_guard<std::mutex> lock(g_Recvmutex);
				for (int i = 0; i < MAX_USER; ++i) {
					if (g_players[i].GetOnline()) {
						if (socketinfo->id == i) {
							//EnterCriticalSection(&g_cs);
							send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
							send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), len, 0);
						}
						//LeaveCriticalSection(&g_cs);
					}
				}
			}
			delete scpacket;
		}
					  break;
		case CS_HIT: {
			CS_HIT_PACKET* cspacket = reinterpret_cast<CS_HIT_PACKET*>(p);

			for (int i = 0; i < MAX_USER; i++)
			{
				if (cspacket->id == g_players[i].GetId())
				{
					if (!cspacket->freeze_bullet) {
						std::lock_guard<std::mutex> lock(g_Recvmutex);
						int tempHP = g_players[i].GetHp() - 10;
						g_players[i].SetHp(tempHP);
						std::cout << "일반포탄 체력감소 적용" << std::endl;
					}
					else {
						std::lock_guard<std::mutex> lock(g_Recvmutex);
						int tempHP = g_players[i].GetHp() - 10;
						g_players[i].SetSpeed(g_players[i].GetSpeed() - 0.1);
						std::cout << "프리즈팩 포탄 이속감소 적용" << std::endl;
					}
					if (g_players[i].GetHp() == 0) {
						//플레이어사망, 게임 종료 패킷 전송
						//여기 오면 do_send도 이제 그만하고, recv도 그만한다. 또한 메인도 종료한다.
						SC_DIE_PACKET* scpacket = new SC_DIE_PACKET;
						scpacket->type = SC_DIE_PLAYER;
						scpacket->id = i;
						{
							std::lock_guard<std::mutex> lock(g_Recvmutex);
							for (int i = 0; i < MAX_USER; ++i) {
								if (g_players[i].GetOnline()) {
									//EnterCriticalSection(&g_cs);
									send(g_players[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
									send(g_players[i].GetSocket(), reinterpret_cast<char*>(scpacket), len, 0);
									//LeaveCriticalSection(&g_cs);
								}
							}
						}
						delete scpacket;
						std::cout << "사망패킷 전송, 게임종료 " << std::endl;
						{
							std::lock_guard<std::mutex> lock(g_Recvmutex);
							g_GameEnd = true;
						}
						break;
					}
					std::cout << g_players[i].GetId() << "번쨰 플레이어 남은 체력 : " << g_players[i].GetHp() << std::endl;
					std::cout << g_players[i].GetId() << "번쨰 플레이어 이동속도 : " << g_players[i].GetSpeed() << std::endl;
				}
			}

		}
				   break;
		default: {
			std::cout << "로그인 오류 - 클라이언트에서 프로세스를 강제종료하였습니다." << std::endl;
			EnterCriticalSection(&g_clientThreadcs);
			--g_ClientNum;
			
			/*g_players[clientIndex].SetId(clientIndex);
			g_players[clientIndex].SetAcceptPlayer(true);*/
			LeaveCriticalSection(&g_clientThreadcs);
			return 0;
		}
			   break;
		}
		//버퍼,길이 초기화
		memset(buf, 99, sizeof(buf));
		len = 0;



	}
	return 0;
}


void tank_collid(std::array<Session, MAX_USER>& players)
{
	std::cout << "in" << std::endl;
	if (collision_Chk(players[0].GetX() - 0.7, players[0].GetX() + 0.7, players[0].GetZ() - 0.7, players[0].GetZ() + 0.7,
		players[1].GetX() - 0.7, players[1].GetX() + 0.7, players[1].GetZ() - 0.7, players[1].GetZ() + 0.7))
	{
		std::cout << "out" << std::endl;
		players[0].SetCollision(true);
		players[1].SetCollision(true);
	}
}

//==충돌 체크=============================================================================================
bool collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB) {

	if (bB <= aT || bT >= aB || bR <= aL || bL >= aR) return false;
	return true;
}

void wall_collid(std::array<Session, MAX_USER>& players, short id)
{
	int id_ = static_cast<int>(id);
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if (Block[i][j].exist)
			{
				if (collision_Chk(Block[i][j].x - 1.0, Block[i][j].x + 1.0, Block[i][j].z - 2.0, Block[i][j].z + 2.0,
					players[id_].GetX() - 0.8, players[id_].GetX() + 0.8, players[id_].GetZ() - 0.5, players[id_].GetZ() + 0.5))
				{
					players[id_].SetWallCollision(true);
				}
			}
		}
	}
}

