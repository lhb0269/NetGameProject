#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include "Enemy.h"
#include "BulletManager.h"
#include "HP.h"

#define MAX_MOB 30

enum PACKET_TYPE {
	PLAYERINFO, UIPACKET, LOBBYPACKET, CLIENTINFO, ALLPACKET
};

enum class COLLIDE_TYPE {
	NON_COLLIDE,
	SWORD_TO_ENEMY,
	SWORD_TO_ENEMYS_BULLET,
	BULLET_TO_ENEMY,
	BULLET_TO_ENEMYS_BULLET,
	ENEMYS_BULLET_TO_PLAYER,
	ENEMYS_BOMB_TO_PLAYER,
};

struct PREPARE_INFO {
	PACKET_TYPE packet_type;
	int collide_ememy_num;
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

struct ALL_PACKET {
	PlayerInfo	P_info[4];
	Enemy		enemyList[MAX_MOB];
	Bullet		bulletList[MAX_BULLET];
	UINT		mob_num;
	UINT		bullet_num;
	UI			Ui[4];
};

struct CollideInfo {
	int index;
	COLLIDE_TYPE collide_type;
};

struct ClientInfo {
	PlayerInfo	Pinfo;
	UI			Ui;
};