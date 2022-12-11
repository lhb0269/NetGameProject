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

	vector<HANDLE> Thread_UpdateEvent;
	HANDLE ServerEvent;

	PlayerInfo recvPlayerInfo;
	UI ui[4];

	ALL_PACKET allPacket;
	ClientInfo Clientinfo;
	vector<CollideInfo> Collideinfo;
	vector<CollideInfo> PlayerCollideinfo;

	HANDLE RecvEvent;
	HANDLE SendEvent;
	HANDLE UpdateEvent;

public:
	int Init();
	int Update();
	void Spawn();
	void Processing(SOCKET& client_sock, HANDLE& ThreadEvent);

	const vector<HANDLE>& GetThreadUpdateEvent() { return Thread_UpdateEvent; }
	const HANDLE& GetServerEvent() { return ServerEvent; }

	SOCKET& GetClinetSock() { return client_sock; }
	void Send_AllPacket();
	void Recv_Packet(SOCKET& clientsock);

	void ClientLogin(SOCKET& clientsock, UINT& nThreadEvent, HANDLE& ThreadEvent);
	EnemyManager* getList();

	void UpdateInitVariable();
	void UpdateMovement();
	void UpdateFrequent();
	void UpdateImmediately();

#ifdef TEST__DEBUG_TIMER_SETTING
	LARGE_INTEGER timer, start, end;

	float GetTime();
	void SetTimer();
#endif
};