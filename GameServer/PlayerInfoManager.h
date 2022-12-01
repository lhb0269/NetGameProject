#pragma once
#include "global.h"

class PlayerInfoManager {
private:
	PlayerInfo pInfo[MAX_PLAYER];
	Sword sInfo[MAX_PLAYER];
	int player_num;

public:
	PlayerInfoManager();
	~PlayerInfoManager();

	void InitPlayer(int pNum);
	void RecvPlayer(SOCKET& clientsock);

	void SetPlayerNum(int num);
	int GetPlayerNum();

	PlayerInfo* HandOverInfo();
};