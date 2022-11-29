#include "stdafx.h"
#include "UIManager.h"

void UIManager::init(Player* player)
{
	ui[player->GetId()].PlayerID = player->GetId();
}

void UIManager::Recv_UI(UI& rui)
{
	memcpy(&ui, &rui, sizeof(UI));
}

void UIManager::UpdateLevel(Player* player, WaveManager* waveMng)
{
	ui[player->GetId()].Stage = waveMng->getLevel();
}

void UIManager::UpdateScore(Player* player)
{
	ui[player->GetId()].score += 10;
}
