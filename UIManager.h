#pragma once
#include"stdafx.h"
#include"Packet.h"
#include"WaveManager.h"
class UIManager
{
	UI ui;

public:
	UIManager();
	~UIManager();

	void init(Player*player);
	UI HandOverInfo();
	void Recv_UI(UI& rui);
	void Send_Msg();
	void UpdateLevel(Player* player,int in);
	void UpdateScore(Player* player,int in);
	void Print();
};

