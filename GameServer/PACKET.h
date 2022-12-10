#pragma once
#include <Windows.h>
#include "Enemy.h"
#include "HP.h"

#define MAX_MOB 30

enum PACKET_TYPE {
	PLAYERINFO, UIPACKET, LOBBYPACKET, COLLIDEENEMY, CLIENTINFO, ALLPACKET
};

struct PlayerInfo {
	int			id;
	int			numOfShell;
	clock_t		isTouched;
	LONG		orbitRay;
	float		shellStack;
	BOOL		isdamaged;
	int			bangMotion;
	POINT		Bangpos;
	POINTFLOAT	velocity;
	POINT		pos;
	Sword		sword;
	HP			hp;
	bool		ready;
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

struct CollideEnemy {
	int Enemyid = -1;
	int Playerid;
	POINT pos;
};
struct ALL_PACKET {
	PlayerInfo	P_info[4];
	Enemy		enemyList[MAX_MOB];
	UI			Ui[4];
	CollideEnemy ce;
};


struct ClientInfo {
	PlayerInfo	Pinfo;
	UI			Ui;
	CollideEnemy ce;
};