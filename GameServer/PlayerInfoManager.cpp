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
	pInfo[Recv_pInfo.id].collision		= Recv_pInfo.collision;
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
	pInfo[pinfo.id].gameover		= pinfo.gameover;
	pInfo[pinfo.id].collision		= pinfo.collision;
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

void PlayerInfoManager::UpdateCollide()
{
	//player sword -> player
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (pInfo[i].id == -1) continue;
		for (int j = 0; j < MAX_PLAYER; ++j)
		{
			if (i == j || pInfo[j].id == -1) continue;
			LKM::Shape fsword(6);
			setSwordShape(fsword, pInfo[i].sword); //fsword 생성

			LKM::Shape swordCollide(6);
			getSwordCollider(&swordCollide, fsword, pInfo[i].sword, pInfo[i].pos); //coliide값 갱신
			
			//충돌체크
			if (beAttacked(&swordCollide, pInfo[j].pos) && pInfo[j].isdamaged && pInfo[j].collision == false)
			{
				pInfo[j].collision = true;
			}
		}
	}
}

BOOL PlayerInfoManager::beAttacked(const LKM::Shape* hitBox,POINT pos)
{
	LKM::Shape temp(8);
	for (int i = 0; i < temp.nPt; ++i) {
		LKM::shapeRegularPlg(20, pos, temp);
	}
	return temp.Collide2Shape(hitBox);
}


void PlayerInfoManager::getSwordCollider(LKM::Shape* hitBox, LKM::Shape& fsword,Sword& sword, POINT pos)
{
	if (hitBox->nPt < fsword.nPt)
		return;
	LKM::Shape& swordc = *hitBox;
	for (int i = 0; i < swordc.nPt; ++i) {
		swordc.ptls[i] = fsword.ptls[i];
	}
	if (sword.isSlash) {
		if (sword.nowAngle - sword.lastAngle > 0) {
			swordc.ptls[0].x += (LONG)(cos(sword.nowAngle) * (10.0f));
			swordc.ptls[0].y -= (LONG)(sin(sword.nowAngle) * (10.0f));
			swordc.ptls[1] = LKM::rotatePoint(pos, swordc.ptls[1], sword.lastAngle - sword.nowAngle);
			swordc.ptls[2] = LKM::rotatePoint(pos, swordc.ptls[2], sword.lastAngle - sword.nowAngle);
			swordc.ptls[3] = pos;
		}
		else {
			swordc.ptls[0].x += (LONG)(cos(sword.nowAngle) * (10.0f));
			swordc.ptls[0].y -= (LONG)(sin(sword.nowAngle) * (10.0f));
			swordc.ptls[5] = LKM::rotatePoint(pos, swordc.ptls[5], sword.lastAngle - sword.nowAngle);
			swordc.ptls[4] = LKM::rotatePoint(pos, swordc.ptls[4], sword.lastAngle - sword.nowAngle);
			swordc.ptls[3] = pos;
		}
	}
}

void PlayerInfoManager::setSwordShape(LKM::Shape& fsword, Sword& sword)
{
	if (fsword.ptls != nullptr)
	{
		fsword.ptls[0] = { sword.swordPos.x + sword.swordRay + sword.swordLength, sword.swordPos.y };
		fsword.ptls[1] = { fsword.ptls[0].x - sword.swordWidth * 2, fsword.ptls[0].y + sword.swordWidth };
		fsword.ptls[2] = { sword.swordPos.x + sword.swordRay, fsword.ptls[1].y };
		fsword.ptls[3] = { sword.swordPos.x + sword.swordRay - sword.swordWidth * 2, sword.swordPos.y };
		fsword.ptls[4] = { fsword.ptls[2].x, fsword.ptls[0].y - sword.swordWidth };
		fsword.ptls[5] = { fsword.ptls[1].x, fsword.ptls[4].y };


		for (int i = 0; i < fsword.nPt; ++i) {
			fsword.ptls[i] = LKM::rotatePoint(sword.swordPos, fsword.ptls[i], sword.nowAngle);
		}
	}
}