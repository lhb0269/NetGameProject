#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include "PACKET.h"
#include "EnemyManager.h"
#include"UIManager.h"
#include "Player.h"

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512

class CLIENT {
private:
	char* SERVERIP = (char*)"127.0.0.1";
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in serveraddr;
	HANDLE hThread;

	EnemyManager* enemyMng;
	Player* player;
	Player* Otherplayers;
	PlayerInfo pInfo;

	PlayerBulletManager *OtherPlayerBullets;
	UI uinfo;
	ALL_PACKET All_packet;
	UIManager UIMng;
	UI uiinfo;

	HANDLE ReadEvent;
	HANDLE SendEvent;
public:
	void err_quit(const char* msg);
	void err_display(const char* msg);
	
	SOCKET& GetSock() { return sock; }

	int Init(Player* p, EnemyManager* e,Player* o,UIManager* u, PlayerBulletManager* b);

	void Login();
	void Send_Packet(void* pakcet,int size,PACKET_TYPE type);
	void Send_Packet(PACKET_TYPE type);
	void Recv_Packet(SOCKET& sock);
	void UpdatePlayerInfo();
	void UpdateUIInfo(int level, int score);
	void UpdateOtherPlayers();
	void UpdateOtherPlayerBullets(RECT *map);
};