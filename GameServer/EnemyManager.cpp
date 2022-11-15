#include"global.h"

void Enemy::setPos(int xpos, int ypos)
{
	x = xpos;
	y = ypos;
}
void Enemy::setState(bool state)
{
	bDead = state;
}
bool Enemy::getState()
{
	return bDead;
}
EnemyManager::EnemyManager()
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		EnemyList[i] = NULL;
	}
	for (int i = 0; i < 10; ++i) {
		typeList[i] = i % 6 ? false : true;
	}
	mobNum = 0;
}
EnemyManager::~EnemyManager()
{
	for (int i = 0; i < mobNum; ++i)
		delete EnemyList[i];
}
void EnemyManager::init()
{
	
}

void EnemyManager::Recv()
{

}

void EnemyManager::EnemyState(int enemyid,bool state)
{
	EnemyList[enemyid]->setState(state);
}

void EnemyManager::Move()
{
	for (int i = 0; i < MAX_ENEMY; ++i) {
		if (EnemyList[i]->getState()) {

		}
	}
}

void EnemyManager::spawn(const POINT spawnPos, int typeSwitch, bool isProtect)
{
	if (mobNum >= MAX_ENEMY)
		return;
	switch (typeSwitch) {
	case HEADED:
		EnemyList[mobNum] = new HeadedMob;
	}
}
