#include "global.h"
#include<iostream>
#include "PlayerInfoManager.h"
std::random_device rd;
std::mt19937_64 re(rd());
std::uniform_int_distribution<int> uid(100, 2900);

#define WHOLE_MAP 2500

//BulletManager EnemyManager::* bulletMng;


EnemyManager::EnemyManager()
{

	//boss = new Sun;
}


EnemyManager::~EnemyManager()
{
	DeleteCriticalSection(&cs);
	/*for (int i = 0; i < mobNum; ++i)
		delete enemyList[i];*/
	//delete bulletMng;
	delete bulletMng;
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
	enemyList[mobNum]->SetState(be_spawn);
	enemyList[mobNum]->SetPrePos(enemyList[mobNum]->getPos());

	//printf("mobnum= %d, x = %d, y = %d angle = %d, force.x = %f, force.y = %f, size = %ld, velocity.x = %f, velocity.y = %f, weight = %f \n", mobNum, enemyList[mobNum]->getPos().x, enemyList[mobNum]->getPos().y, enemyList[mobNum]->getAngle(), enemyList[mobNum]->getForce().x, enemyList[mobNum]->getForce().y, enemyList[mobNum]->getSize(), enemyList[mobNum]->getVelocity().x, enemyList[mobNum]->getVelocity().y, enemyList[mobNum]->getWeight());
	mobNum++;

#ifdef TEST__SWORD_TO_ENEMY_PRT
	std::cout << "������ �޸� [" << mobNum - 1 << "] = " << enemyList[mobNum - 1] << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < mobNum; ++i)
	{
		if (enemyList[i])
			std::cout << "���� �޸� [" << i << "] = " << enemyList[i] << std::endl;
	}
	std::cout << std::endl;
#endif
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
			targetPlayer[i] = shortest_n;
			enemyList[i]->move(pInfo[shortest_n].pos);
			/*if (!PtInRect(&WholeMapRect, enemyList[i]->getPos())) {
				if (enemyList[i]->goOut()) {
					destroy(i);
				}
			}*/
		}
	}
	RECT map = { 0, 0, WHOLE_MAP, WHOLE_MAP };
	EnterCriticalSection(&cs);
	bulletMng->moveAll(&map);
	LeaveCriticalSection(&cs);

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
void EnemyManager::shoot()
{
	for (int i = 0; i < mobNum; ++i) {
		if (enemyList[i]->isSpawned() && enemyList[i]->isShoot()) {
			enemyList[i]->bang(playerMng->HandOverInfo()[targetPlayer[i]].pos, *bulletMng, typeList[nowBulletType]);
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

//Bomber* EnemyManager::getBomb(int index)
//{
//	return dynamic_cast<Bomber*>(enemyList[index]);
//}

void EnemyManager::init()
{
	InitializeCriticalSection(&cs);

	for (int i = 0; i < MAX_MOB; ++i) {
		enemyList[i] = NULL;
		targetPlayer[i] = 0;
	}
	for (int i = 0; i < 10; ++i) {
		typeList[i] = i % 6 ? false : true;
	}
	nowBulletType = 0;
	mobNum = 0;
}

void EnemyManager::UpdateCollide(std::vector<CollideInfo>& ce)
{
	for (int i = 0; i < ce.size(); ++i)
	{
		switch (ce.back().collide_type)
		{
		case COLLIDE_TYPE::SWORD_TO_ENEMY:
		{
			int index = ce.back().index;

#ifdef TEST__SWORD_TO_ENEMY_PRT
			int count = std::count_if(ce.begin(), ce.end(), [index](const CollideInfo& cinfo) { return cinfo.index == index; });
			std::cout << "���� �ε��� �� [" << index << "] = " << count << std::endl;
			if (count > 1) std::cout << "\n\n\n\n------delete error------\n\n\n\n" << std::endl;
			std::cout << "����� �޸� [" << index << "] = " << enemyList[index] << std::endl;
			std::cout << "������ �޸� [" << mobNum << "] = " << enemyList[mobNum] << std::endl << std::endl;
#endif
			POINT previous_pos = enemyList[index]->getPos();
			*enemyList[index] = *enemyList[--mobNum];
			enemyList[index]->SetState(be_destroyed);
			enemyList[index]->SetPrePos(previous_pos);
			delete enemyList[mobNum];
		}
		break;
		case COLLIDE_TYPE::SWORD_TO_ENEMYS_BULLET:
		{
			EnterCriticalSection(&cs);
			int index = ce.back().index;
			POINT previous_pos = bulletMng->getBulletPtr(index)->getPos();
			int bullet_type = bulletMng->getBulletPtr(index)->igetType();
			bulletMng->destroy(index);
			if (bullet_type == NORMAL)
				bulletMng->getBulletPtr(index)->SetState(particle_nomal);
			else
				bulletMng->getBulletPtr(index)->SetState(particle_super);
			bulletMng->getBulletPtr(index)->SetPrePos(previous_pos);
			LeaveCriticalSection(&cs);
		}
		break;
		case COLLIDE_TYPE::BULLET_TO_ENEMY:
		{
			int index = ce.back().index;

#ifdef TEST__SWORD_TO_ENEMY_PRT
			int count = std::count_if(ce.begin(), ce.end(), [index](const CollideInfo& cinfo) { return cinfo.index == index; });
			std::cout << "���� �ε��� �� [" << index << "] = " << count << std::endl;
			if (count > 1) std::cout << "\n\n\n\n------delete error------\n\n\n\n" << std::endl;
			std::cout << "����� �޸� [" << index << "] = " << enemyList[index] << std::endl;
			std::cout << "������ �޸� [" << mobNum << "] = " << enemyList[mobNum] << std::endl << std::endl;
#endif
			POINT previous_pos = enemyList[index]->getPos();
			* enemyList[index] = *enemyList[--mobNum];
			enemyList[index]->SetState(be_destroyed);
			enemyList[index]->SetPrePos(previous_pos);
			delete enemyList[mobNum];
		}
		break;
		case COLLIDE_TYPE::BULLET_TO_ENEMYS_BULLET:
		{
			EnterCriticalSection(&cs);
			int index = ce.back().index;
			POINT previous_pos = bulletMng->getBulletPtr(index)->getPos();
			int bullet_type = bulletMng->getBulletPtr(index)->igetType();
			bulletMng->destroy(index);
			if (bullet_type == NORMAL)
				bulletMng->getBulletPtr(index)->SetState(particle_nomal);
			else
				bulletMng->getBulletPtr(index)->SetState(particle_super);
			bulletMng->getBulletPtr(index)->SetPrePos(previous_pos);
			LeaveCriticalSection(&cs);
		}
		break;
		case COLLIDE_TYPE::ENEMYS_BULLET_TO_PLAYER:

			break;
		case COLLIDE_TYPE::ENEMYS_BOMB_TO_PLAYER:
			break;
		default:
			break;
		}
		ce.pop_back();
	}
}

void EnemyManager::UpdateState()
{
	for (int i = 0; i < MAX_MOB; ++i)
	{
		if (enemyList[i]) enemyList[i]->SetState(none);
	}
	for (int i = 0; i < MAX_BULLET; ++i)
	{
		if (bulletMng->getBulletPtr(i)) bulletMng->getBulletPtr(i)->SetState(none);
	}
}

void EnemyManager::EnemtState(const Enemy& enemy)
{

}
Enemy** EnemyManager::HandOverInfo()
{
	return enemyList;
}

Enemy* EnemyManager::HandOverInfo(int n)
{
	return enemyList[n];
}