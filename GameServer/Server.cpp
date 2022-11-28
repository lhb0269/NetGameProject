#pragma once
#include "Server.h"
#include "global.h"

int SERVER::Init()
{
	enemyManager->init();
	int retval = 0;

	playerMng->InitPlayer(ClientCount);

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	ReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	SendEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
}


DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SERVER* server = (SERVER*)arg;
	SOCKET client_sock = server->GetClinetSock();

	server->ClientLogin(client_sock);
	while (1) {
		//send recv 구현필요
		server->Recv_Packet(client_sock);
		server->Send_AllPacket();

	}

	// 소켓 닫기
	return 0;
}

void SERVER::Recv_Packet(SOCKET& clientsock)
{
	WaitForSingleObject(SendEvent, INFINITE);
	int retval;
	//패킷 type 이랑 크기 받고 type에 따라 처리해야함
	PACKET_TYPE type;
	retval = recv(clientsock, (char*)&type, sizeof(type), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("recv()");

	switch (type)
	{
	case PLAYERINFO:
	{
		if (ClientCount != playerMng->GetPlayerNum())
			playerMng->SetPlayerNum(ClientCount);

		playerMng->RecvPlayer(clientsock);
		break;
	}
	case UIPACKET:
		UIMng->Recv_UI(clientsock);
		break;
	case LOBBYPACKET:
		break;
	case COLLIDEENEMY:
	{
		retval = recv(clientsock, (char*)&recvCollide, sizeof(CollideEnemy), MSG_WAITALL);
		if (retval == SOCKET_ERROR) err_display("recv()");
		enemyManager->Recv(recvCollide);
		break;
	}
	case ALLPACKET:
		break;
	}
	playerMng->printPlayerInfo();

	SetEvent(ReadEvent);
}

void SERVER::Send_AllPacket()
{
	waveMng->update();
	Spawn();
	//플레이어 받아오면 player->getcore() 넘겨준다.
	enemyManager->move({ 50,50,50,50 });

	WaitForSingleObject(ReadEvent, INFINITE);
	ALL_PACKET packet;
	memcpy(packet.P_info, playerMng->HandOverInfo(), sizeof(PlayerInfo) * MAX_PLAYER);
	memcpy(packet.enemyList, enemyManager->HandOverInfo(), sizeof(Enemy) * MAX_MOB);
	memcpy(packet.Ui, UIMng->HandOverInfo(), sizeof(UI) * MAX_PLAYER);

#ifdef TEST__SEND_ALLPACKET__PINFO_POS
	for (int i = 0; i < ClientCount; ++i)
	{
		cout << packet.P_info->id << ": " <<
			packet.P_info->pos.x << ", " << packet.P_info->pos.y << endl;
	}
#endif

	for (auto& cl : v_clients)
		send(cl, (char*)&packet, sizeof(packet), 0);
	SetEvent(SendEvent);

}

void SERVER::ClientLogin(SOCKET& clientsock)
{
	int retval;
	v_clients.push_back(clientsock);
	retval = send(clientsock, (char*)&ClientCount, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	ClientCount++;
}

EnemyManager* SERVER::getList()
{
	return enemyManager;
}
void SERVER::Spawn()
{
	int mobNum = enemyManager->getEnemyNumber();

	if (waveMng->isSpawn()) {
		Monster* now = waveMng->getMob();
		if (now != nullptr) {
			enemyManager->spawn(now->start, now->type, now->protect);
		}
	}

	/*static int shootTerm = 100;
	static int shootStock = shootTerm;
	shootStock++;
	if (shootStock > shootTerm) {
		shootStock = 0;
		enemyManager->shoot(player.getPos());
	}*/
}

int SERVER::Update()
{
	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)this, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
		
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}