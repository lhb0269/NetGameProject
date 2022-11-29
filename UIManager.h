#pragma once
#include"stdafx.h"
#include"Packet.h"
//#include"TeamProject_2KnMn_1.h"
class UIManager
{
	UI ui[4];

public:
	UIManager();
	~UIManager();

	void init(Player*player);
	UI* HandOverInfo();
	void Recv_UI(UI& rui);
	void Send_Msg();
	void UpdateLevel(Player* player,WaveManager* waveMng);
	void UpdateScore(Player* player);
};

