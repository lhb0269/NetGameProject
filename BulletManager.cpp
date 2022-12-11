#include "stdafx.h"
#include "BulletManager.h"
#include "EffectManager.h"

extern EffectManager effectMng;

BulletManager::BulletManager()
{
	numBullet = 0;
	for (int i = 0; i < MAX_BULLET; ++i)
	{
		bulletList[i] = NULL;
	}
}


BulletManager::~BulletManager()
{
	for (int i = 0; i < numBullet; ++i) {
		delete bulletList[i];
	}
	numBullet = 0;
}

int BulletManager::getBulletNum()
{
	return numBullet;
}

void BulletManager::setBulletNum(int n)
{
	numBullet = n;
}

Bullet* BulletManager::getBulletPtr(int index)
{
	return bulletList[index];
}

void BulletManager::drawAll(HDC hdc, COLORREF backcolor)
{
	for (int i = 0; i < numBullet; ++i) {
		if(bulletList[i]->visible)
			bulletList[i]->draw(hdc, backcolor);
	}
}

void BulletManager::addBullet(POINT start, POINT target, bool type)
{
	if (numBullet < MAX_BULLET) {
		bulletList[numBullet++] = new Bullet(start, target, type ? SUPER : NORMAL);
	}
}

void BulletManager::addBullet(const Bullet& blet, int n)
{
	bulletList[n] = new Bullet();
	*bulletList[n] = blet;
}

void BulletManager::destroy(int index) {
	bulletList[index] = bulletList[--numBullet];
	delete bulletList[numBullet];
}

void BulletManager::moveAll(RECT* camera, RECT* map)
{
	for (int i = 0; i < numBullet; ++i) {
		bulletList[i]->move(camera);
		if (!PtInRect(map, bulletList[i]->getPos())) {
			destroy(i);
		}
	}
}

int BulletManager::isCollideToSword(const LKM::Shape * sword)
{
	int result = -1;
	for (int i = 0; i < numBullet; ++i) {
		Bullet& bullet = *bulletList[i];
		if (bullet.visible) {
			if (bullet.collideTo(sword)) {
				result = i;
				//effectMng.add(bullet.getPos(), bullet.getType() ? Particle_Normal : Particle_Super );
				destroy(i);
				break;
			}
		}
	}
	return result;
}

int BulletManager::isCollideToBullet(const LKM::Shape * bullet)
{
	int result = -1;
	for (int i = 0; i < numBullet; ++i) {
		Bullet& now = *bulletList[i];
		if (now.visible && now.getType()) {
			if (now.collideTo(bullet)) {
				result = i;
				//effectMng.add(now.getPos(), now.getType() ? Particle_Normal : Particle_Super);
				destroy(i);
				break;
			}
		}
	}
	return result;
}
