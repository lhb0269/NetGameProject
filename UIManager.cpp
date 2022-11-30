#include "stdafx.h"
#include "UIManager.h"
UIManager::UIManager()
{

}
UIManager::~UIManager()
{

}
void UIManager::init(Player* player)
{
	ui.PlayerNum = NULL;
	ui.PlayerID = player->GetId();
	ui.score = NULL;
	ui.Stage = NULL;
}

UI UIManager::HandOverInfo()
{
	return ui;
}

void UIManager::Recv_UI(UI& rui)
{
	memcpy(&ui, &rui, sizeof(UI) * 4);
}

void UIManager::UpdateLevel(Player* player, int in)
{
	ui.PlayerID = player->GetId();
	ui.Stage = in;
}

void UIManager::UpdateScore(Player* player, int in)
{
	ui.score = in;
}
void UIManager::Print()
{
	/*for (int i = 0; i < 4; ++i) {
		printf("id = %d\n", i);
		printf("stage = %d\n", ui[i].Stage);
		printf("score = %d\n", ui[i].score);
	}*/
}