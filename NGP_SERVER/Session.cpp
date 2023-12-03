#include "Session.h"
#include "Collision.h"

Session::Session()
{
	m_socketinfo = new SocketInfo;
	m_x = 0;
	m_z = 0;
	m_hp = 0;
	m_speed = 0;
	m_online = false;
	m_bullet_cnt = 0;
	m_accept_player = false;
	m_ready_player = false;
	m_collision = false;
	m_win = false;

}

Session::~Session()
{
	delete m_name;
	delete m_socketinfo;
}

void Session::SetSocket(SOCKET socket) { m_socketinfo->client_socket = socket; }


void Session::SetX(float x) { m_x = x; }

void Session::SetY(float y){m_y = y;}

void Session::SetId(short id) { m_socketinfo->id = id; }

void Session::SetZ(float z) { m_z = z; }


void Session::SetHp(short hp) { m_hp = hp; }

void Session::SetSpeed(float speed) { m_speed = speed; }

void Session::SetName(char* name) { 
	m_name = new char[strlen(name) + 1];
	strcpy(m_name, name); 
}

void Session::SetOnline(bool online) { m_online = online; }

void Session::setBulletCnt(short bullet_cnt) { m_bullet_cnt = bullet_cnt; }

void Session::SetAcceptPlayer(bool accept_player) { m_accept_player = accept_player; }

void Session::SetReadyPlayer(bool ready_player) { m_ready_player = ready_player; }

void Session::setYaw(float yaw) { m_yaw = yaw; }


SOCKET Session::GetSocket() const { return m_socketinfo->client_socket; }

float Session::GetX() const{ return m_x; }

float Session::GetY() const { return m_y; }

float Session::GetZ() const{ return m_z; }

short Session::GetHp() const{ return m_hp; }

short Session::GetId() const { return m_socketinfo->id; }

float Session::GetSpeed() const{ return m_speed; }

SocketInfo* Session::GetSocketInfo() const { return m_socketinfo; }

bool Session::GetOnline() const{ return m_online; }


short Session::GetBulletCnt() const{ return m_bullet_cnt; }

bool Session::GetAcceptPlayer() const { return m_accept_player; }

char* Session::GetName() const { return m_name; }

bool Session::GetReadyPlayer() const { return m_ready_player; }

float Session::GetYaw() const { return m_yaw; }

//bool Session::collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB)
//{
//	if (bB <= aT || bT >= aB || bR <= aL || bL >= aR) return false;
//	return true;
//	std::cout << "collision" << std::endl;
//}
//
//bool Session::mov_coliiCHK(float x, float y, float z)
//{
//	for (int i = 0; i < 30; i++)
//	{
//		for (int j = 0; j < 30; j++)
//		{
//			if (Block[i][j].exist)
//			{
//				if (collision_Chk(Block[i][j].x - 1.0, Block[i][j].x + 1.0, Block[i][j].z - 2.0, Block[i][j].z + 2.0,
//					x - 0.5, x+ 0.5, z - 0.5, z + 0.5))
//				{
//
//					return true;
//				}
//			}
//
//		}
//
//	}
//	return false;
//}



