#pragma once
#define MAX_ENEMY 100
class Enemy
{
	int x, y;//��ǥ
public:
	void setPos(int x,int y);
};
class EnemyManager
{
	Enemy EnemyList[100];
public:
	void init();//EnemyList����
	void Recv();//CollideEnemy �浹������ �޴´�.
	void EnemyState();//Enemy�� ������ �ް� �ش� Enemy�� ������ ����.
	void Move();//Enemy���� �̵� ���
};