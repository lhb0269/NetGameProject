#include "stdafx.h"
#include "EnemyManager.h"
#include "Boss.h"

#include"Clinet.h"
#include <time.h>
std::random_device rd;
std::mt19937_64 re(rd());
std::uniform_int_distribution<int> uid(100, 2900);

extern MapManager mapMng;

BulletManager EnemyManager::* bulletMng;
EffectManager effectMng;

EnemyManager::EnemyManager()
{
	for (int i = 0; i < MAX_MOB; ++i) {
		enemyList[i] = NULL;
	}

	for (int i = 0; i < 10; ++i) {
		typeList[i] = i % 6 ? false : true;
	}
	nowBulletType = 0;
	mobNum = 0;
	//boss = new Sun;
}


EnemyManager::~EnemyManager()
{
	for (int i = 0; i < mobNum; ++i)
		delete enemyList[i];
	delete bulletMng;
}

void EnemyManager::draw(HDC hdc)
{
	for (int i = 0; i < mobNum; ++i)
		if (enemyList[i] && enemyList[i]->isSpawned())
		{
			enemyList[i]->draw(hdc);
		}
	bulletMng->drawAll(hdc, mapMng.getThemaColor());
	effectMng.draw(hdc);
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i] && !enemyList[i]->isSpawned()) {
			enemyList[i]->spawnSignal();
		}
	}
	//if (boss != nullptr)
	//	boss->draw(hdc);
}

void EnemyManager::move(const RECT& player)
{
	POINT playerSize = { player.right - player.left, player.bottom - player.top };
	POINT playerPos = { player.left + playerSize.x / 2, player.top + playerSize.y / 2 };
	RECT WholeMapRect = mapMng.getWholeMapRect();
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i] && enemyList[i]->isSpawned()) {
			enemyList[i]->move(playerPos);
			if (!PtInRect(&WholeMapRect, enemyList[i]->getPos())) {
				//if (enemyList[i]->goOut()) {
				//	destroy(i);
				//}
			}
		}
	}
	RECT camera = mapMng.getCameraRect();
	RECT map = mapMng.getWholeMapRect();
	bulletMng->moveAll(&camera, &map);

	//if (boss != nullptr) {
	//	boss->move(playerPos);
	//}
}

void EnemyManager::destroy(int index)
{
	effectMng.add(enemyList[index]->getPos(), Destroy);
	delete enemyList[index];
	enemyList[index] = enemyList[--mobNum];
	//index ������ ������ enemylist���� ó��

}

void EnemyManager::Recv(Enemy* recvList)
{
	memcpy(enemyList, recvList, sizeof(enemyList));
}

int EnemyManager::isAttacked(const LKM::Shape* bitBox)
{
	int result = -1;
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i]->isSpawned() && enemyList[i]->beAttacked(bitBox)) {
			if (!enemyList[i]->isProtect()) {
				effectMng.add(enemyList[i]->getPos(), BOSS_DAMAGED);
				//destroy(i);
				result = i;
				i--;
			}
			else {
				result = i;
				enemyList[i]->protectOnOff(false);
				effectMng.add(enemyList[i]->getPos(), Break);
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

void EnemyManager::shoot(POINT player)
{
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i]->isSpawned() && enemyList[i]->isShoot()) {
			enemyList[i]->bang(player, *bulletMng, typeList[nowBulletType]);
			nowBulletType++;
			nowBulletType %= 10;
		}
	}
	//if (boss != nullptr) {
	//	boss->bang(player, bulletMng, typeList[nowBulletType]);
	//}
}


int EnemyManager::getEnemyNumber()
{
	return mobNum;
}

Bomber* EnemyManager::getBomb(int index)
{
	return dynamic_cast<Bomber*>(enemyList[index]);
}

void EnemyManager::EnemyInfoUpdate(const Enemy* enm, const Bullet* blet)
{
	for (int i = 0; i < bulletMng->getBulletNum(); ++i)
	{
		bulletMng->addBullet(blet[i], i);
		switch (blet[i].GetState())
		{
		case particle_nomal:
			effectMng.add(blet[i].GetPrePos(), Particle_Normal);
			break;
		case particle_super:
			effectMng.add(blet[i].GetPrePos(), Particle_Super);
			break;
		}
	}
	for (int i = 0; i < mobNum; ++i)
	{
		if (!enemyList[i])
		{
			switch (enm[i].getShape()) {
			case HEADED:
				enemyList[i] = new HeadedMob();
				break;
			case HEADLESS:
				enemyList[i] = new HeadlessMob();
				break;
			case TOWER:
				enemyList[i] = new Tower();
				break;
			case BOMBER:
				enemyList[i] = new Bomber();
				break;
			case SLUG:
				enemyList[i] = new Slug();
				break;
			case SUN:
				enemyList[i] = new Sun();
				break;
			default:
				enemyList[i] = new HeadedMob();
				break;
			}
			//mobNum++;
		}
		switch (enm[i].GetState())
		{
		case be_spawn:
			effectMng.add(enm[i].GetPrePos(), Create);
			break;
		case be_breaken:
			effectMng.add(enm[i].GetPrePos(), Break);
			break;
		case be_destroyed:
			effectMng.add(enm[i].GetPrePos(), Destroy);
			break;
		}
		*enemyList[i] = enm[i];
		enemyList[i]->UpdateBody();	//현재 HeadedMob만 만들어둠.
	}
}

void EnemyManager::SetMosterType(int n)
{

}
