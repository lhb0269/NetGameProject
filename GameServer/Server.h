#pragma once
#include <stdio.h>
#include <iostream>
#include "global.h"

#define SERVERPORT 9000
#define BUFSIZE    512

class SERVER {
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKET client_sock;
	vector<SOCKET> v_clients;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;
	char addr[INET_ADDRSTRLEN];
	char buf[BUFSIZE + 1];
	EnemyManager* enemyManager = new EnemyManager;
	WaveManager* waveMng = new WaveManager;
	PlayerInfoManager* playerMng = new PlayerInfoManager;
	UIManager* UIMng = new UIManager;
	int ClientCount = 0;

	PlayerInfo playerInfo[4];
	PlayerInfo recvPlayerInfo;
	UI ui[4];
	CollideEnemy recvCollide;

	ALL_PACKET allPacket;

	HANDLE ReadEvent;
	HANDLE SendEvent;
public:
	int Init();
	int Update();
	void Spawn();

	SOCKET& GetClinetSock() { return client_sock; }
	void Send_AllPacket();
	void Recv_Packet(SOCKET& clientsock);

	void ClientLogin(SOCKET& clientsock);
	void UpdateAllPacket();
	void printPlayerInfo();
	EnemyManager* getList();

};