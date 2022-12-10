#pragma once
#include "Enemy.h"
#include "Player.h"

enum PACKET_TYPE {
	PLAYERINFO,UIPACKET, LOBBYPACKET, CLIENTINFO, ALLPACKET
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
	Bullet		bulletList[MAX_BULLET];
	UINT		mob_num;
	UINT		bullet_num;
	UI			Ui[4];
};

class CollideInfo {
private:
	int index;
	COLLIDE_TYPE collide_type;
public:
	CollideInfo(int idx, COLLIDE_TYPE cld_typ) :index(idx), collide_type(cld_typ) {}
};

struct ClientInfo {
	PlayerInfo	Pinfo;
	UI			Ui;
};