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
	void Recv_UI(UI& rui);
	void Send_Msg();
};

