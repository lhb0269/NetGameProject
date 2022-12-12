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
	void RecvPlayer(PlayerInfo& pinfo);

	void SetPlayerNum(int num);
	int GetPlayerNum();
	bool getReady();

	PlayerInfo* HandOverInfo();
	void UpdateCollide(std::vector<CollideInfo>& ce);
	void UpdateCollide();
	BOOL beAttacked(const LKM::Shape* hitBox,POINT pos);
	void setSwordShape(LKM::Shape& fsword,Sword& sword);
	void getSwordCollider(LKM::Shape* hitBox, LKM::Shape& fsword,Sword& sword,POINT pos);
};