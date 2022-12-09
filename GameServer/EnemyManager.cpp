#include "global.h"
#include<iostream>
std::random_device rd;
std::mt19937_64 re(rd());
std::uniform_int_distribution<int> uid(100, 2900);


//BulletManager EnemyManager::* bulletMng;

EnemyManager::EnemyManager()
{

	//boss = new Sun;
}


EnemyManager::~EnemyManager()
{
	/*for (int i = 0; i < mobNum; ++i)
		delete enemyList[i];*/
		//delete bulletMng;
}

//void EnemyManager::draw(HDC hdc)
//{
//	for (int i = 0; i < mobNum; ++i)
//		if (enemyList[i]->isSpawned())
//			enemyList[i]->draw(hdc);
//
//	bulletMng->drawAll(hdc, mapMng.getThemaColor());
//	effectMng.draw(hdc);
//	for (int i = 0; i < mobNum; ++i) {
//		if (!enemyList[i]->isSpawned()) {
//			enemyList[i]->spawnSignal();
//		}
//	}
//	//if (boss != nullptr)
//	//	boss->draw(hdc);
//}
void EnemyManager::spawn(const POINT spawnPos, int typeSwitch, bool isProtect)
{
	if (mobNum >= MAX_MOB)
		return;
	switch (typeSwitch) {
	case HEADED:
		enemyList[mobNum] = new HeadedMob();
		break;
	case HEADLESS:
		enemyList[mobNum] = new HeadlessMob();
		break;
	case TOWER:
		enemyList[mobNum] = new Tower();
		break;
	case BOMBER:
		enemyList[mobNum] = new Bomber();
		break;
	case SLUG:
		enemyList[mobNum] = new Slug();
		break;
	default:
		enemyList[mobNum] = new HeadedMob();
		break;
	}
	enemyList[mobNum]->start(spawnPos);
	enemyList[mobNum]->protectOnOff(isProtect);
	enemyList[mobNum]->setShape(typeSwitch);
	if (!enemyList[mobNum]->isSpawned())
		enemyList[mobNum]->spawnSignal();
	//printf("mobnum= %d, x = %d, y = %d angle = %d, force.x = %f, force.y = %f, size = %ld, velocity.x = %f, velocity.y = %f, weight = %f \n", mobNum, enemyList[mobNum]->getPos().x, enemyList[mobNum]->getPos().y, enemyList[mobNum]->getAngle(), enemyList[mobNum]->getForce().x, enemyList[mobNum]->getForce().y, enemyList[mobNum]->getSize(), enemyList[mobNum]->getVelocity().x, enemyList[mobNum]->getVelocity().y, enemyList[mobNum]->getWeight());
	mobNum++;
	//effectMng.add(spawnPos, Create);
}

void EnemyManager::move(const PlayerInfo* pInfo)
{
	LONG x = 0;
	LONG y = 0;
	const LONG size = 20;	// player half size - 20


	//RECT WholeMapRect = mapMng.getWholeMapRect();
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i]->isSpawned()) {
			int shortest_n = 0;
			for (int n = 0, k = INT_MAX; n < MAX_PLAYER; ++n)
			{
				int len = pow((pInfo[n].pos.x - enemyList[i]->getPos().x), 2) + pow((pInfo[n].pos.y - enemyList[i]->getPos().y), 2);
				if (len < k)
				{
					k = len;
					shortest_n = n;
				}
			}
			enemyList[i]->move(pInfo[shortest_n].pos);
			/*if (!PtInRect(&WholeMapRect, enemyList[i]->getPos())) {
				if (enemyList[i]->goOut()) {
					destroy(i);
				}
			}*/
		}
	}
	//RECT camera = mapMng.getCameraRect();
	//RECT map = mapMng.getWholeMapRect();
	//bulletMng->moveAll(&camera, &map);

	//if (boss != nullptr) {
	//	boss->move(playerPos);
	//}
}

//void EnemyManager::destroy(int index)
//{
//	//effectMng.add(enemyList[index]->getPos(), Destroy);
//	delete enemyList[index];
//	enemyList[index] = enemyList[--mobNum];
//}

BOOL EnemyManager::isAttacked(const LKM::Shape* bitBox)
{
	bool result = false;
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i]->isSpawned() && enemyList[i]->beAttacked(bitBox)) {
			if (!enemyList[i]->isProtect()) {
				//effectMng.add(enemyList[i]->getPos(), BOSS_DAMAGED);
				//destroy(i);
				i--;
				result = true;
			}
			else {
				result = true;
				enemyList[i]->protectOnOff(false);
				//effectMng.add(enemyList[i]->getPos(), Break);
			}
			break;
		}
	}
	//if (boss != nullptr && boss->beAttacked(bitBox)) {
	//	result = true;
	//	effectMng.add(boss->getPos(), BOSS_DAMAGED);
	//	delete boss;
	//	boss = nullptr;
	//}
	return result;
}
//
//void EnemyManager::shoot(POINT player)
//{
//	for (int i = 0; i < mobNum; ++i) {
//		if (enemyList[i]->isSpawned() && enemyList[i]->isShoot()) {
//			enemyList[i]->bang(player, *bulletMng, typeList[nowBulletType]);
//			nowBulletType++;
//			nowBulletType %= 10;
//		}
//	}
//	//if (boss != nullptr) {
//	//	boss->bang(player, bulletMng, typeList[nowBulletType]);
//	//}
//}


int EnemyManager::getEnemyNumber()
{
	return mobNum;
}

//Bomber* EnemyManager::getBomb(int index)
//{
//	return dynamic_cast<Bomber*>(enemyList[index]);
//}

void EnemyManager::init()
{
	for (int i = 0; i < MAX_MOB; ++i) {
		enemyList[i] = NULL;
	}
	for (int i = 0; i < 10; ++i) {
		typeList[i] = i % 6 ? false : true;
	}
	nowBulletType = 0;
	mobNum = 0;
}

void EnemyManager::Recv(ClientInfo* ci)
{
	if (ci->ce.Enemyid != -1 && enemyList[ci->ce.Enemyid] != NULL) {
		delete enemyList[ci->ce.Enemyid];
		enemyList[ci->ce.Enemyid] = enemyList[--mobNum];
	}
}

void EnemyManager::EnemtState(const Enemy& enemy)
{

}
Enemy* EnemyManager::HandOverInfo(int n)
{
	return enemyList[n];
}