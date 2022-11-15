#include "EnemyManager.h"
#include"PACKET.h"

void Enemy::setPos(int xpos, int ypos)
{
	x = xpos;
	y = ypos;
}
void EnemyManager::init()
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		EnemyList[i].setPos(1,1);
	}
}

void EnemyManager::Recv()
{

}

void EnemyManager::EnemyState()
{

}

void EnemyManager::Move()
{

}
