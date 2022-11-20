#include "stdafx.h"
#include "Clinet.h"

void CLIENT::err_quit(const char* msg)
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
void CLIENT::err_display(const char* msg)
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

DWORD WINAPI RecvThread(LPVOID arg)
{
	int retval = 0;
	CLIENT* pClient = (CLIENT*)arg;
	SOCKET sock = pClient->GetSock();
	char buf[BUFSIZE];

	pClient->Login();
	while (true)
	{
		pClient->Recv_Packet(sock);
	}
}

void CLIENT::Login()
{
	int retval = 0;
	int id = 0;
	retval = recv(sock, (char*)&id, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("send()");

	player->SetId(id);
}

void CLIENT::Send_Packet(void* pakcet,int size,PACKET_TYPE type)
{
	int retval;
	retval = send(sock, (char*)type, sizeof(type), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	retval = send(sock, (char*)pakcet, size, 0);
	if (retval == SOCKET_ERROR) err_display("send()");
}

void CLIENT::Send_Packet(PACKET_TYPE type)
{
	int retval = 0;
	switch (type)
	{
	case PLAYERINFO:
	{
		UpdatePlayerInfo();
		retval = send(sock, (char*)&type, sizeof(PACKET_TYPE), 0);
		retval = send(sock, (char*)&pInfo, sizeof(PlayerInfo), 0);
		break;
	}
	case UIPACKET:
		break;
	case LOBBYPACKET:
		break;
	case COLLIDEENEMY:
		break;
	case ALLPACKET:
		break;
	}
	if (retval == SOCKET_ERROR) err_display("send()");
}

void CLIENT::UpdatePlayerInfo()
{
	pInfo.id = player->GetId();
	pInfo.pos = player->getPos();
	pInfo.sword = player->getSword();
}

void CLIENT::Recv_Packet(SOCKET& sock)
{
	int retval;
	ALL_PACKET packet;

	retval = recv(sock, (char*)&packet, sizeof(ALL_PACKET), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("send()");
}

int CLIENT::Init(Player* p, EnemyManager* e)
{
	player = p;
	enemyMng = e;

	int retval;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 家南 积己
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	hThread = CreateThread(NULL, 0, RecvThread,
		(LPVOID)this, 0, NULL);
	if(hThread != NULL) (CloseHandle(hThread));
}