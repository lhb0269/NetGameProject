#pragma once
#define MAX_ENEMY 100
#include<Windows.h>
class Enemy
{
	int x, y;//좌표
	bool bDead = false;
public:
	void setPos(int x,int y);
	void setState(bool state);
	bool getState();
};
class EnemyManager
{
	Enemy* EnemyList[MAX_ENEMY];
	int mobNum;
	bool typeList[10];
public:
	EnemyManager();
	~EnemyManager();
	void init();//EnemyList생성
	void Recv();//CollideEnemy 충돌정보를 받는다.
	void EnemyState(int enemyid,bool state);//Enemy의 정보를 받고 해당 Enemy의 정보를 갱신.
	void Move();//Enemy들의 이동 담당
	void spawn(const POINT spawnPos, int typeSwitch, bool isProtect);
};