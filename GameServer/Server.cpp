#pragma once
#include "Server.h"
#include "global.h"
void SERVER::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void SERVER::err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
int SERVER::Init()
{
	enemyManager->init();
	int retval = 0;

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
		//send recv �����ʿ�
		//server->Recv_Packet(client_sock);
		server->Send_AllPacket();
	}

	// ���� �ݱ�
	return 0;
}

void SERVER::Recv_Packet(SOCKET& clientsock)
{
	WaitForSingleObject(SendEvent, INFINITE);
	int retval;
	//��Ŷ type �̶� ũ�� �ް� type�� ���� ó���ؾ���
	PACKET_TYPE type;
	retval = recv(clientsock, (char*)&type, sizeof(type), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("recv()");

	switch (type)
	{
	case PLAYERINFO:
	{
		retval = recv(clientsock, (char*)&recvPlayerInfo, sizeof(PlayerInfo), MSG_WAITALL);
		if (retval == SOCKET_ERROR) err_display("recv()");
		playerInfo[recvPlayerInfo.id].pos = recvPlayerInfo.pos;
		playerInfo[recvPlayerInfo.id].sword = recvPlayerInfo.sword;
		break;
	}
	case UIPACKET:
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
	printPlayerInfo();

	SetEvent(ReadEvent);
}

void SERVER::Send_AllPacket()
{
	waveMng->update();
	Spawn();
	//�÷��̾� �޾ƿ��� player->getcore() �Ѱ��ش�.
	enemyManager->move({ 50,50,50,50 });
	//WaitForSingleObject(ReadEvent, INFINITE);
	ALL_PACKET packet;
	//memcpy(packet.enemyList, enemyManager->HandOverInfo(), sizeof(Enemy*) * MAX_MOB);
	for (auto& cl : v_clients)
		send(cl, (char*)&packet, sizeof(packet), 0);
	//SetEvent(SendEvent);

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
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)this, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
		
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}

void SERVER::printPlayerInfo()
{
	for (int i = 0; i < ClientCount; ++i)
	{
		cout << playerInfo[i].id << ": " <<
			playerInfo[i].pos.x << ", " << playerInfo[i].pos.y << endl;
	}
}