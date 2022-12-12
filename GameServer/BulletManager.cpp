#include "global.h"
#include "BulletManager.h"
//#include "EffectManager.h"

//extern EffectManager effectMng;

BulletManager::BulletManager()
{
	for (int i = 0; i < MAX_BULLET; ++i)
		bulletList[i] = NULL;
	numBullet = 0;
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

Bullet * BulletManager::getBulletPtr(int index)
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

void BulletManager::destroy(int index)
{
	*bulletList[index] = *bulletList[--numBullet];
	delete bulletList[numBullet];
}

void BulletManager::moveAll(RECT* map)
{
	for (int i = 0; i < numBullet; ++i) {
		bulletList[i]->move();
		if (!PtInRect(map, bulletList[i]->getPos())) {
			destroy(i);
		}
	}
}

BOOL BulletManager::isCollideToSword(const LKM::Shape * sword)
{
	bool result = false;
	for (int i = 0; i < numBullet; ++i) {
		Bullet& bullet = *bulletList[i];
		if (bullet.visible) {
			if (bullet.collideTo(sword)) {
				result = true;
				//effectMng.add(bullet.getPos(), bullet.getType() ? Particle_Normal : Particle_Super );
				destroy(i);
				break;
			}
		}
	}
	return result;
}

BOOL BulletManager::isCollideToBullet(const LKM::Shape * bullet)
{
	bool result = false;
	for (int i = 0; i < numBullet; ++i) {
		Bullet& now = *bulletList[i];
		if (now.visible && now.getType()) {
			if (now.collideTo(bullet)) {
				result = true;
				//effectMng.add(now.getPos(), now.getType() ? Particle_Normal : Particle_Super);
				destroy(i);
				break;
			}
		}
	}
	return result;
	return 0;
}
