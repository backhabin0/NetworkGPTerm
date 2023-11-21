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

#pragma pack(push,1)

struct CS_LOGIN_PACKET {
	char type;
	int id;
	char name[NAME_SIZE];
};

struct CS_READY_PACKET {
	char type;
	char name[NAME_SIZE];
};
struct CS_MOVE_PACKET {
	char type;
	char direction;
};


struct CS_ITEM_PACKET {
	char type;
	char item;
};

struct CS_LOGOUT_PACKET {
	char tpye;
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
};

struct SC_READY_OK_PACKET {
	char type;
	char name[NAME_SIZE];
};
struct SC_LOGINFAIL_PACKET {
	char type;
};

struct SC_MOVE_PACKET {
	char type;
	int id;
	float x, y, z;
};

struct SC_DIE_PACKET {
	char type;
};

struct SC_STATE_CHANGE_PACKET {
	char type;
	int id;
	char state;
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
};

#pragma pack (pop)