#pragma once
#include "Object.h"

enum BULLET_TYPE {
	NORMAL = 0xf000, SUPER, PARRYING
};

class Bullet : public RealObject
{
	static FLOAT maxSpeed;
	static LONG size;
	int type;

	COLORREF getColor();
public:
	bool visible;
	Bullet() :type{ NORMAL }, visible{ false } {};
	Bullet(POINT start, POINT target, int Type);
	~Bullet();
	RECT getRect();
	LONG getSize();
	bool getType();
	int igetType();
	void draw(HDC, COLORREF);
	void move();
	bool collideTo(const LKM::Shape*);
};


#define MAX_BULLET 300

class BulletManager
{
	Bullet* bulletList[MAX_BULLET];
	int numBullet;

public:
	BulletManager();
	~BulletManager();
	int getBulletNum();
	Bullet* getBulletPtr(int index);
	void destroy(int i);
	void moveAll(RECT* map);
	BOOL isCollideToSword(const LKM::Shape* sword);
	BOOL isCollideToBullet(const LKM::Shape* bullet);
	void drawAll(HDC, COLORREF);
	void addBullet(POINT start, POINT target, bool type);
};