#pragma once
#include <Windows.h>
#include "Enemy.h"


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
	Enemy enemyList;
	UI Ui;
};

struct CollideEnemy {
	int Enemyid;
	int Playerid;
	POINT pos;
};