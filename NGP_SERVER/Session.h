#pragma once

#include "protocol.h"

//#define HP 100
//#define X 10
//#define Y 10
//#define SPEED 0.3
//#define BULLET_CNT 10

struct SocketInfo {
	SOCKET client_socket;
	short id;
};

class Session {
private:
	SocketInfo* m_socketinfo;

	char* m_name;
	float m_x, m_y, m_z;	//��ũ ��ǥ
	short m_hp;		//��ũ ü��
	float m_speed;	//��ũ �ӵ�	
	bool m_online;	// ���� ���� ����
	bool m_ready;
	short m_bullet_cnt;	//��ũ �Ѿ� ��
	bool m_accept_player;	// ���� ���� ����
	bool m_ready_player=false;	// 
	float m_yaw;
public:

	Session();
	~Session();

	void SetSocket(SOCKET socket);
	void SetId(short id);
	void SetName(char* name);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float y);
	void SetHp(short hp);
	void SetSpeed(float speed);
	void SetOnline(bool online);
	void setBulletCnt(short bullet_cnt);
	void SetAcceptPlayer(bool accept_player);
	void SetReadyPlayer(bool ready_player);
	void setYaw(float yaw);

	SOCKET GetSocket() const;
	SocketInfo* GetSocketInfo() const;
	short GetId() const;
	char* GetName() const;
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	short GetHp() const;
	float GetSpeed() const;
	bool GetOnline() const;
	short GetBulletCnt() const;
	bool GetAcceptPlayer() const;
	bool GetReadyPlayer() const;
	float GetYaw() const;

	bool collision_Chk(float aL, float aR, float aT, float aB, float bL, float bR, float bT, float bB);
	bool mov_coliiCHK(float x, float y, float z);
};