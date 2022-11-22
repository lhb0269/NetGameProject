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
	Sword sword; //Ŭ���̾�Ʈ class SWORD�� �����;���
};

struct UI {
	int PlayerNum;	// ���� �������� �÷��̾� ��
	int State;		// ���� �������� Stage �ܰ�
	int MSG_TYPE;	//
};

struct LOBBYPACKET {
	int		PlayerCount;
	bool	Ready, Start;
	int		id;
};

struct ALL_PACKET {
	PlayerInfo	P_info[4];
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