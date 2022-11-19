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
}


DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SERVER* server = (SERVER*)arg;
	SOCKET client_sock = server->GetClinetSock();

	server->ClientLogin(client_sock);
	while (1) {
		//send recv 구현필요
	}

	// 소켓 닫기
	closesocket(client_sock);
	return 0;
}

void SERVER::Recv_Packet()
{

}

void SERVER::Send_AllPacket()
{

}

void SERVER::ClientLogin(SOCKET& clientsock)
{
	int retval;
	retval = send(clientsock, (char*)&ClientCount, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");
	ClientCount++;
	std::cout << ClientCount << std::endl;
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
	waveMng->update();
	Spawn();
	//플레이어 받아오면 player->getcore() 넘겨준다.
	enemyManager->move({ 50,50,50,50 });
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