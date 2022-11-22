#pragma once
#include <Windows.h>
#include "Enemy.h"
#define MAX_MOB 100

enum PACKET_TYPE {
	PLAYERINFO, UIPACKET, LOBBYPACKET, COLLIDEENEMY, ALLPACKET
};

struct PlayerInfo {
	int id;
	POINT pos;
	Sword sword; //클라이언트 class SWORD를 가져와야함
};

struct UI {
	int PlayerNum;	// 현재 접속중인 플레이어 수
	int State;		// 현재 진행중인 Stage 단계
	int MSG_TYPE;	//
};

struct LOBBYPACKET {
	int		PlayerCount;
	bool	Ready, Start;
	int		id;
};

struct ALL_PACKET {
	PlayerInfo	P_info[MAX_PLAYER];
	Enemy *enemyList[MAX_MOB];

	UI Ui;
};

struct CollideEnemy {
	int Enemyid;
	int Playerid;
	POINT pos;
};

struct state {
	int nHeaded = 0;
	int nHeadlessMob = 0;
	int nTower = 0;
	int nBomber = 0;
	int nSlug = 0;
};