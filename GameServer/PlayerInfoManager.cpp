#include "PlayerInfoManager.h"
#include <iostream>

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
		pInfo[i].id = -1;
		pInfo[i].pos = { 0, 0 };
	}
}

void PlayerInfoManager::RecvPlayer(SOCKET& clientsock)
{
	PlayerInfo Recv_pInfo;
	int retval = recv(clientsock, (char*)&Recv_pInfo, sizeof(PlayerInfo), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("recv()");

	pInfo[Recv_pInfo.id].id = Recv_pInfo.id;
	pInfo[Recv_pInfo.id].pos = Recv_pInfo.pos;
	pInfo[Recv_pInfo.id].sword = Recv_pInfo.sword;
}

void PlayerInfoManager::SetPlayerNum(int num)
{
	player_num = num;
}

int PlayerInfoManager::GetPlayerNum()
{
	return player_num;
}

PlayerInfo* PlayerInfoManager::HandOverInfo()
{
	return pInfo;
}

void PlayerInfoManager::ErrorInfoCheck()
{
}

void PlayerInfoManager::printPlayerInfo()
{
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		std::cout << pInfo[i].id << ": " <<
			pInfo[i].pos.x << ", " << pInfo[i].pos.y << endl;
	}
}
