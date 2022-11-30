#pragma once
#include "Monster.h"
#include "Tower.h"
#include"global.h"
class EnemyManager
{
	Enemy enemyList[MAX_MOB];
	int mobNum;
	bool typeList[10];
	int nowBulletType;
public:
	//BulletManager* bulletMng = new BulletManager;

	EnemyManager();
	~EnemyManager();
	//void draw(HDC hdc);
	void spawn(const POINT spawnPos, int monsyer, bool isProtect);
	void move(const RECT& player);
	BOOL isAttacked(const LKM::Shape* sword);
	//void shoot(POINT player);
	int getEnemyNumber();
	Bomber* getBomb(int index);
	//void destroy(int index);
	//Server
	void init();
	void Recv(const CollideEnemy& ce);
	void EnemtState(const Enemy& enemy);
	Enemy* HandOverInfo();
};