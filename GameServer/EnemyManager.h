#pragma once
#define MAX_ENEMY 100
#include<Windows.h>
class Enemy
{
	int x, y;//��ǥ
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
	void init();//EnemyList����
	void Recv();//CollideEnemy �浹������ �޴´�.
	void EnemyState(int enemyid,bool state);//Enemy�� ������ �ް� �ش� Enemy�� ������ ����.
	void Move();//Enemy���� �̵� ���
	void spawn(const POINT spawnPos, int typeSwitch, bool isProtect);
};