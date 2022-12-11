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

	PlayerBulletManager* OtherPlayerBullets;
	ALL_PACKET All_packet;
	UIManager UIMng;
	UI uiinfo;

	ClientInfo Clientinfo;
	vector<CollideInfo> Collideinfo;
	vector<CollideInfo> PlayerCollideinfo;
	RECT mapsize;

	HANDLE ReadEvent;
	HANDLE SendEvent;
public:
	void err_quit(const char* msg);
	void err_display(const char* msg);

	SOCKET& GetSock() { return sock; }

	int Init(Player* p, EnemyManager* e, Player* o, UIManager* u, PlayerBulletManager* b);

	void Login();
	void Send_Packet(void* pakcet, int size, PACKET_TYPE type);
	void Send_Packet(PREPARE_INFO pre_info);
	void Recv_Packet(SOCKET& sock);
	
	ClientInfo* GetClientInfo() { return &Clientinfo; }
	vector<CollideInfo>& GetCollideInfo() { return Collideinfo; }
	vector<CollideInfo>& GetPlayerCollideInfo() { return PlayerCollideinfo; }
	void SetMapSize(RECT r) { mapsize = r; }
	RECT& GetMapSize() { return mapsize; }

	void UpdatePlayerInfo();
	void UpdateUIInfo(int level);
	void UpdateOtherPlayers();
	void UpdateOtherPlayerBullets(RECT* map);
	void UpdateEnemy();
	void UpdateClientUiInfo();
	void UpdateClientPacketData();
	void UpdateScore(int score);
	void printUI(POINT& point,HDC hdc);
	void setReady();
	bool getReady();
	bool AllReady();
	bool GameSet();
};