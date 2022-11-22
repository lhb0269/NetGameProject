#ifndef _GWINSOCK_H_
#define _GWINSOCK_H_

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <windows.h>

void err_quit(const char* msg);

void err_display(const char* msg);

#endif