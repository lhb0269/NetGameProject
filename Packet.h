#pragma once
#include "Enemy.h"
#include "Player.h"

enum PACKET_TYPE {
	PLAYERINFO,UIPACKET,LOBBYPACKET, COLLIDEENEMY,ALLPACKET
};

struct PlayerInfo {
	int id;
	POINT pos;
	Sword sword;
};

struct UI {
	int PlayerNum;	
	int State;		
	int MSG_TYPE;	
};

struct LOBBYPACKET {
	int		PlayerCount;
	bool	Ready, Start;
	int		id;
};

struct ALL_PACKET {
	PlayerInfo	P_info[4];
	Enemy enemyList[100];
	UI			Ui;
};

struct CollideEnemy {
	int Enemyid;
	int Playerid;
	POINT pos;
};