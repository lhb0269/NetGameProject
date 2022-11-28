#pragma once
#include <Windows.h>
#include "Enemy.h"
#define MAX_MOB 100

enum PACKET_TYPE {
	PLAYERINFO, UIPACKET, LOBBYPACKET, COLLIDEENEMY, ALLPACKET
};

struct PlayerInfo {
	int id;
	int numOfShell;
	clock_t isTouched;
	LONG orbitRay;
	float shellStack;
	POINT pos;
	Sword sword;
};

struct UI {
	int PlayerNum;	// 현재 접속중인 플레이어 수
	int Stage;		// 현재 진행중인 Stage 단계
	int score;
	int MSG_TYPE;	
	int PlayerID;
};

struct LOBBYPACKET {
	int		PlayerCount;
	bool	Ready, Start;
	int		id;
};

struct ALL_PACKET {
	PlayerInfo	P_info[4];
	Enemy enemyList[100];
	UI Ui[4];
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