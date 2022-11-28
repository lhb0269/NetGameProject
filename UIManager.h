#pragma once
#include"stdafx.h"
#include"Packet.h"
class UIManager
{
	UI ui;

public:
	UIManager();
	~UIManager();

	void init();
	UI* HandOverInfo();
	void Recv_UI(UI& rui);
	void Send_Msg();
};

