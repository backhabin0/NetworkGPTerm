#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#include <array>
#include <iostream>

#define HP 100
#define X 10
#define Y 10
#define SPEED 0.3
#define BULLET_CNT 10

#define SERVERPORT 9000
#define BUFSIZE    512

enum DATA_TYPE { ACCEPT, SEND, RECV };
enum COMP_TYPE { OP_ATTACK, OP_STATE_CAHGE, OP_MOVE };
enum ITEM { HEAL, SPEEDUP, FREEZE };
enum DIRECTION { UP, DOWN, LEFT, RIGHT };

constexpr int MAX_USER = 2;
constexpr int NAME_SIZE = 40;

// Packet ID
constexpr char CS_LOGIN = 0;
constexpr char CS_READY = 1;
constexpr char CS_MOVE = 2;
constexpr char CS_ATTACK = 3;
constexpr char CS_ITEM = 4;
constexpr char CS_LOGOUT = 5;
constexpr char CS_YAW = 6;
constexpr char CS_ATTACK_END = 7;
constexpr char CS_HIT = 8;


constexpr char SC_LOGIN_INFO = 0;
constexpr char SC_LOGIN_OK = 1;
constexpr char SC_LOGIIN_FAIL = 2;
constexpr char SC_READY_OK = 3;
constexpr char SC_MOVE_OBJECT = 4;
constexpr char SC_DIE_OBJECT = 5;
constexpr char SC_STATE_CHANGE = 6;
constexpr char SC_ATTACK = 7;
constexpr char SC_LOGOUT = 8;
constexpr char SC_GAMESTART = 9;
constexpr char SC_PLAYING_TIME = 10;
constexpr char SC_UPDATE = 11;
constexpr char SC_SET_ITEM = 12;
constexpr char SC_ALL_ITEM_SET = 13;

#pragma pack(push,1)

struct CS_LOGIN_PACKET {
	char type;
	char name[NAME_SIZE];
};

struct CS_READY_PACKET {
	char type;
};
struct CS_MOVE_PACKET {
	char type;
	char direction;
};

struct CS_ATTACK_PACKET {
	char tpye;
	float now_yaw;
	float x;
	float z;
	bool isshoot;
};

struct CS_ATTACK_END_PACKET {
	char tpye;
	float now_yaw;
	float x;
	float z;
	bool isshoot;
};

struct CS_ITEM_PACKET {
	char type;
	char item;
	bool exist;
	int num;
};

struct CS_LOGOUT_PACKET {
	char tpye;
};

struct CS_YAW_PACKET {
	char type;
	float yaw;
};

struct CS_HIT_PACKET {
	char type;
	int id;
	int hp;
};

struct SC_LOGIN_INFO_PACKET {
	char type;
	int id;
	int hp;
	short x, y;
	float speed;
	short bullet_cnt;
};

struct SC_LOGIN_OK_PACKET {
	char type;
	char name[NAME_SIZE];
	int id;
	int hp;
	float x;
	float y;
	float z;
	float speed;
	short bullet_cnt;
};

struct SC_READY_PACKET {
	char type;
};
struct SC_LOGINFAIL_PACKET {
	char type;
};

struct SC_MOVE_PACKET {
	char type;
	int id;
	short x, y, z;
};

struct SC_DIE_PACKET {
	char type;
};

struct SC_STATE_CHANGE_PACKET {
	char type;
	int id;
	char state;
};

struct SC_ATTACK_PACKET {
	char type;
	int id;
	float now_yaw;
	float x;
	float z;
	bool isshoot;
};


struct SC_READY_OK_PACKET {
	char type;
	char name[NAME_SIZE];
};

struct SC_UPDATE_PACKET {
	char type;
	short id;
	short hp;
	float x;
	float y;
	float z;
	float speed;
	short bullet_cnt;
	float yaw;
};

struct SC_SET_ITEM_PACKET {
	char type;
	char item_type;
	bool exist[3];
	float x[3];
	float z[3];
};

struct SC_ALL_ITEM_SET_PACKET {
	char type;
};
#pragma pack (pop)