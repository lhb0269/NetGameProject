#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include"global.h"
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
	int ClientCount = 0;

	PlayerInfo playerInfo[4];
	PlayerInfo recvPlayerInfo;
	CollideEnemy recvCollide;
	ALL_PACKET allPacket;

	HANDLE ReadEvent;
	HANDLE SendEvent;
public:
	int Init();
	int Update();
	void Spawn();
	void err_quit(const char* msg);
	void err_display(const char* msg);

	SOCKET& GetClinetSock() { return client_sock; }
	void Send_AllPacket();
	void Recv_Packet(SOCKET& clientsock);

	void ClientLogin(SOCKET& clientsock);
	void UpdateAllPacket();
	void printPlayerInfo();
	EnemyManager* getList();
};