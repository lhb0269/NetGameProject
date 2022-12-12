#include "global.h"
#include "BulletManager.h"
#include "Enemy.h"


LONG Enemy::getSize()
{
	return size;
}

bool Enemy::isSpawned()
{
	return isSpawn;
}

void Enemy::spawnSignal()
{
	/*spawnStack++;
	if (spawnStack > 15) {
	}*/
	isSpawn = true;
}

bool Enemy::isProtect()
{
	return bProtected;
}

void Enemy::setShape(int num)
{
	shape = num;
}

//bool Enemy::getBulletType()
//{
//	type %= 10;
//	return typeList[type++];
//}

RECT Enemy::getBody()
{
	RECT body = { pos.x - size, pos.y - size, pos.x + size, pos.y + size };
	return body;
}

void Enemy::drawProtector(HDC hdc)
{
	if (bProtected) {
		LONG orbitRay = size * 2;
		RECT shell = { pos.x - orbitRay, pos.y - orbitRay, pos.x + orbitRay, pos.y + orbitRay };
		const int TermAngle = 15;
		HPEN pen = CreatePen(PS_SOLID, 5, RGB(0xff, 0xff, 0xff));
		HPEN  oldpen = (HPEN)SelectObject(hdc, pen);
		HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, shell.left, shell.top, shell.right, shell.bottom);
		SelectObject(hdc, oldpen);
		SelectObject(hdc, oldbrush);
		DeleteObject(pen);
	}
}

Enemy::Enemy()
{
	size = 20;
	int stock = 3;
	isSpawn = false;
	spawnStack = 0;
	bProtected = false;
}


Enemy::~Enemy()
{
}

void Enemy::draw(HDC)
{
}

void Enemy::move(const POINT& target)
{
}

BOOL Enemy::beAttacked(const LKM::Shape* hitBox)
{
	LKM::Shape temp(8);
	for (int i = 0; i < temp.nPt; ++i) {
		LKM::shapeRegularPlg(size, pos, temp);
	}
	return temp.Collide2Shape(hitBox);
}

BOOL Enemy::start(const POINT& p)
{
	if (isSpawn)
		return false;
	pos = p;
	isSpawn = false;
	return true;
}

BOOL Enemy::goOut()
{
	return false;
}

BOOL Enemy::isShoot()
{
	return true;
}

void Enemy::bang(const POINT & playerPos, BulletManager & bm, bool type)
{
	bm.addBullet(pos, playerPos, type);
}

void Enemy::protectOnOff(bool in)
{
	bProtected = in;
}

