#pragma once
#include "Enemy.h"
#include "Player.h"

enum PACKET_TYPE {
	PLAYERINFO,UIPACKET,LOBBYPACKET, COLLIDEENEMY,CLIENTINFO,ALLPACKET
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
};

struct UI {
	int PlayerNum;	
	int Stage;	
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
	Enemy		enemyList[30];
	UI			Ui[4];
};

struct CollideEnemy {
	int Enemyid;
	int Playerid;
	POINT pos;
};

struct ClientInfo {
	PlayerInfo	Pinfo;
	UI			Ui;
};