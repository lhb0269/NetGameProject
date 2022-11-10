#pragma once
#include <Windows.h>

struct PlayerInfo {
	POINT pos;
	//SWORD sword; //클라이언트 class SWORD를 가져와야함
	int id;
	int direction;
	int speed;
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
	PlayerInfo	P_info[4];
	//ENEMY		EnemyList;
	UI			Ui;
};

struct CollideEnemy {
	int Enemy_id;
	POINT pos;
};