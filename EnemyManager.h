#pragma once
#include "MapManager.h"
#include "EffectManager.h"
#include "Monster.h"
#include "Tower.h"

#define MAX_MOB 100

class EnemyManager
{
	Enemy* enemyList[MAX_MOB];
	int mobNum;
	bool typeList[10];
	int nowBulletType;

public:
	BulletManager* bulletMng = new BulletManager;

	EnemyManager();
	~EnemyManager();
	void draw(HDC hdc);
	void spawn(const POINT spawnPos, int monsyer, bool isProtect);
	void move(const RECT& player);
	BOOL isAttacked(const LKM::Shape* sword);
	void shoot(POINT player);
	int getEnemyNumber();
	Bomber* getBomb(int index);
	void destroy(int index);
	//Server
	void init();
	void Recv();
	void EnemtState(Enemy);
	Enemy* HandOverInfo();
};