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

	pInfo[Recv_pInfo.id].id				= Recv_pInfo.id;
	pInfo[Recv_pInfo.id].pos			= Recv_pInfo.pos;
	pInfo[Recv_pInfo.id].sword			= Recv_pInfo.sword;
	pInfo[Recv_pInfo.id].isTouched		= Recv_pInfo.isTouched;
	pInfo[Recv_pInfo.id].numOfShell		= Recv_pInfo.numOfShell;
	pInfo[Recv_pInfo.id].orbitRay		= Recv_pInfo.orbitRay;
	pInfo[Recv_pInfo.id].shellStack		= Recv_pInfo.shellStack;
	pInfo[Recv_pInfo.id].isdamaged		= Recv_pInfo.isdamaged;
	pInfo[Recv_pInfo.id].bangMotion		= Recv_pInfo.bangMotion;
	pInfo[Recv_pInfo.id].Bangpos		= Recv_pInfo.Bangpos;
	pInfo[Recv_pInfo.id].velocity		= Recv_pInfo.velocity;
	pInfo[Recv_pInfo.id].ready			= Recv_pInfo.ready;
	pInfo[Recv_pInfo.id].hp				= Recv_pInfo.hp;
	pInfo[Recv_pInfo.id].gameover		= Recv_pInfo.gameover;
}

void PlayerInfoManager::RecvPlayer(PlayerInfo& pinfo)
{
	pInfo[pinfo.id].id				= pinfo.id;
	pInfo[pinfo.id].pos				= pinfo.pos;
	pInfo[pinfo.id].sword			= pinfo.sword;
	pInfo[pinfo.id].isTouched		= pinfo.isTouched;
	pInfo[pinfo.id].numOfShell		= pinfo.numOfShell;
	pInfo[pinfo.id].orbitRay		= pinfo.orbitRay;
	pInfo[pinfo.id].shellStack		= pinfo.shellStack;
	pInfo[pinfo.id].isdamaged		= pinfo.isdamaged;
	pInfo[pinfo.id].bangMotion		= pinfo.bangMotion;
	pInfo[pinfo.id].Bangpos			= pinfo.Bangpos;
	pInfo[pinfo.id].velocity		= pinfo.velocity;
	pInfo[pinfo.id].ready			= pinfo.ready;
	pInfo[pinfo.id].hp				= pinfo.hp;	
	pInfo[pinfo.id].gameover = pinfo.gameover;
}

void PlayerInfoManager::SetPlayerNum(int num)
{
	player_num = num;
}

int PlayerInfoManager::GetPlayerNum()
{
	return player_num;
}

bool PlayerInfoManager::getReady()
{
	int j = 0;
	for (int i = 0; i <= player_num; ++i) {
		if (pInfo[i].ready) {
			++j;
			if (j >= player_num)
				return true;
		}
	}
	return false;
}

PlayerInfo* PlayerInfoManager::HandOverInfo()
{
	return pInfo;
}