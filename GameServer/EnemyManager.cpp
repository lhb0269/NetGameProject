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
		enemyList[mobNum].setShape(0);
		break;
	case HEADLESS:
		enemyList[mobNum].setShape(1);
		break;
	case TOWER:
		enemyList[mobNum].setShape(2);
		break;
	case BOMBER:
		enemyList[mobNum].setShape(3);
		break;
	case SLUG:
		enemyList[mobNum].setShape(4);
		break;
	default:
		enemyList[mobNum].setShape(0);
		break;
	}
	enemyList[mobNum].start(spawnPos);
	enemyList[mobNum].protectOnOff(isProtect);
	if (!enemyList[mobNum].isSpawned())
		enemyList[mobNum].spawnSignal();
	//printf("mobnum= %d, x = %d, y = %d angle = %d, force.x = %f, force.y = %f, size = %ld, velocity.x = %f, velocity.y = %f, weight = %f \n", mobNum, enemyList[mobNum]->getPos().x, enemyList[mobNum]->getPos().y, enemyList[mobNum]->getAngle(), enemyList[mobNum]->getForce().x, enemyList[mobNum]->getForce().y, enemyList[mobNum]->getSize(), enemyList[mobNum]->getVelocity().x, enemyList[mobNum]->getVelocity().y, enemyList[mobNum]->getWeight());
	mobNum++;
	//effectMng.add(spawnPos, Create);
}

void EnemyManager::move(const RECT& player)
{
	POINT playerSize = { player.right - player.left, player.bottom - player.top };
	POINT playerPos = { player.left + playerSize.x / 2, player.top + playerSize.y / 2 };
	//RECT WholeMapRect = mapMng.getWholeMapRect();
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i].isSpawned()) {
			enemyList[i].move(playerPos);
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
		if (enemyList[i].isSpawned() && enemyList[i].beAttacked(bitBox)) {
			if (!enemyList[i].isProtect()) {
				//effectMng.add(enemyList[i]->getPos(), BOSS_DAMAGED);
				//destroy(i);
				i--;
				result = true;
			}
			else {
				result = true;
				enemyList[i].protectOnOff(false);
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
		enemyList[i] = Enemy();
	}

	for (int i = 0; i < 10; ++i) {
		typeList[i] = i % 6 ? false : true;
	}
	nowBulletType = 0;
	mobNum = 0;
}

void EnemyManager::Recv(const CollideEnemy& ce)
{
	//delete enemyList[ce.Enemyid];
	enemyList[ce.Enemyid] = enemyList[--mobNum];
}

void EnemyManager::EnemtState(const Enemy& enemy)
{

}
Enemy* EnemyManager::HandOverInfo()
{
	return enemyList;
}