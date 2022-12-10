#pragma once
#include "Monster.h"
#include "Tower.h"
#include"global.h"
class PlayerInfoManager;
class EnemyManager
{
	Enemy* enemyList[MAX_MOB];
	int mobNum;
	bool typeList[10];
	int nowBulletType;
public:
	int targetPlayer[MAX_MOB];
	BulletManager* bulletMng = new BulletManager;
	PlayerInfoManager* playerMng;

	EnemyManager();
	~EnemyManager();
	//void draw(HDC hdc);
	void spawn(const POINT spawnPos, int monsyer, bool isProtect);
	void move(const PlayerInfo* pInfo);
	BOOL isAttacked(const LKM::Shape* sword);
	void shoot();
	int getEnemyNumber();
	Bomber* getBomb(int index);
	//void destroy(int index);
	//Server
	void init();
	void UpdateCollide(std::vector<CollideInfo>& ce);
	void UpdateState();
	void EnemtState(const Enemy& enemy);
	Enemy** HandOverInfo();
	Enemy* HandOverInfo(int n);
};