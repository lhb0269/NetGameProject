#pragma once
#include "global.h"

class PlayerInfoManager {
private:
	PlayerInfo pInfo[MAX_PLAYER];
	Sword sInfo[MAX_PLAYER];
	int player_num;

public:
	EFFECT_INFO effectInfo[MAX_PLAYER];
	int effectNum;

	PlayerInfoManager();
	~PlayerInfoManager();

	void InitPlayer(int pNum);
	void RecvPlayer(SOCKET& clientsock);
	void RecvPlayer(PlayerInfo& pinfo);

	void SetPlayerNum(int num);
	int GetPlayerNum();
	bool getReady();

	PlayerInfo* HandOverInfo();
	void UpdateCollide(std::vector<CollideInfo>& ce);
};