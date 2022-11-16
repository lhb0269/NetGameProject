#pragma once
#include "Player.h"
#include "Enemy.h"

struct PlayerInfo {
	POINT pos;
	Sword sword;
	int id;
	int direction;
	int speed;
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
	Enemy*		EnemyList;
	UI			Ui;
};

struct CollideEnemy {
	int Enemyid;
	int Playerid;
	POINT pos;
};