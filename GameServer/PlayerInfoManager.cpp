#include "PlayerInfoManager.h"

PlayerInfoManager::PlayerInfoManager()
{
}

PlayerInfoManager::~PlayerInfoManager()
{
}

void PlayerInfoManager::InitPlayer(int pNum)
{
	player_num = pNum;

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		// playerInfo init
	}
}

void PlayerInfoManager::RecvPlayer()
{
}

PlayerInfo* PlayerInfoManager::HandOverInfo()
{
	return nullptr;
}

void PlayerInfoManager::ErrorInfoCheck()
{
}
