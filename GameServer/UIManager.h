#pragma once
#include"global.h"
class UIManager
{
	UI ui[MAX_PLAYER];

public:
	UIManager();
	~UIManager();

	void init();
	UI* HandOverInfo();
	void Recv_UI(SOCKET& clientsock);
	void Send_Msg();
};

