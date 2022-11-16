#pragma once
#include <Windows.h>

struct PlayerInfo {
	POINT pos;
	//Sword sword; //Ŭ���̾�Ʈ class SWORD�� �����;���
	int id;
	int direction;
	int speed;
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
	//ENEMY		EnemyList;
	UI			Ui;
};

struct CollideEnemy {
	int Enemyid;
	int Playerid;
	POINT pos;
};