#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE    512

class SERVER {
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKET client_sock;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;
	char addr[INET_ADDRSTRLEN];
	char buf[BUFSIZE + 1];
public:
	int Init();
	int Update();
	void err_quit(const char* msg);
	void err_display(const char* msg);

	SOCKET& GetClinetSock() { return client_sock; }
};