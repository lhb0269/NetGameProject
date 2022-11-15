#pragma once
#define MAX_ENEMY 100
class Enemy
{
	int x, y;//좌표
public:
	void setPos(int x,int y);
};
class EnemyManager
{
	Enemy EnemyList[100];
public:
	void init();//EnemyList생성
	void Recv();//CollideEnemy 충돌정보를 받는다.
	void EnemyState();//Enemy의 정보를 받고 해당 Enemy의 정보를 갱신.
	void Move();//Enemy들의 이동 담당
};