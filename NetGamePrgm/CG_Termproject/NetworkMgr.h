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
#include "protocol.h"
//#include "C:\Users\user\Desktop\�ݰ�������\NGP_TermProject\NetworkGPTerm\NetGamePrgm/error.h"

//#define SERVERPORT 9000
//#define BUFSIZE 512
//#define NAMESIZE 20
//

class NetworkMgr
{
	SOCKET sock;

public:
	NetworkMgr();
	~NetworkMgr();

	SOCKET GetSocket() { return sock; }
	bool Init();
	
public:
	void SendPacket(char* packet,int size);
};
