#pragma once

#include "protocol.h"

#define HP 100
#define X 10
#define Y 10
#define SPEED 10
#define BULLET_CNT 10

struct SocketInfo {
	SOCKET client_socket;
	short id;
};

class Session {
private:
	SocketInfo* m_socketinfo;

	char* m_name;
	float m_x, m_y, m_z;	//攀农 谅钎
	short m_hp;		//攀农 眉仿
	float m_speed;	//攀农 加档	
	bool m_online;	// 蜡历 立加 咯何
	short m_bullet_cnt;	//攀农 醚舅 荐
	bool m_accept_player;	// 蜡历 荐侩 咯何
	bool m_ready_player;	// 
	
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
};