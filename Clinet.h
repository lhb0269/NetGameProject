#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
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
public:
	void err_quit(const char* msg);
	void err_display(const char* msg);
	
	SOCKET& GetSock() { return sock; }
	int Init();
	void Login();
	void Send_Packet();
	void Recv_Packet();
};