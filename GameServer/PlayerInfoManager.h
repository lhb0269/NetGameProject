#pragma once
#include "global.h"

struct Sword {
	FLOAT rotMoment;
	FLOAT rotVelocity;
	LONG inertia;
	int numOfRotate;
	int pnm;
	bool isSlash;
	POINT swordPos;
	LONG swordLength;
	LONG swordLength_s;
	LONG swordWidth;
	FLOAT nowAngle;
	FLOAT lastAngle;
	LONG swordRay;
};

class PlayerInfoManager {
private:
	PlayerInfo pInfo[MAX_PLAYER];
	Sword sInfo[MAX_PLAYER];
	int player_num;

public:
	PlayerInfoManager();
	~PlayerInfoManager();

	void InitPlayer(int pNum);
	void RecvPlayer();
	PlayerInfo* HandOverInfo();
	void ErrorInfoCheck();
};