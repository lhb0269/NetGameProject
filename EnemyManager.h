#pragma once
#include "Tower.h"
#include "MapManager.h"
#include "EffectManager.h"
#include "Monster.h"
#include "Packet.h"

class CLIENT;
class EnemyManager
{
public:
	Enemy* enemyList[MAX_MOB];
	int mobNum;
	bool typeList[10];
	int nowBulletType;

public:
	BulletManager* bulletMng = new BulletManager;

	EFFECT_INFO effectInfo[MAX_MOB];
	int effectNum;

	EFFECT_INFO effectBulletInfo[MAX_BULLET];
	int effectBulletNum;

	EnemyManager();
	~EnemyManager();
	void draw(HDC hdc);
	void move(const RECT& player);
	int isAttacked(const LKM::Shape* sword);
	void shoot(POINT player);
	int getEnemyNumber();
	Bomber* getBomb(int index);
	void destroy(int index);

	//update
	void EnemyInfoUpdate(const Enemy* enm, const Bullet* blet);
	void EnemyEffectUpdate(const EFFECT_INFO* enm, const EFFECT_INFO* blet);
	void SetMosterType(int n);

	//client
	void init();
	void Recv(Enemy* recvList);
	void EnemtState(Enemy);
	Enemy* HandOverInfo();
};