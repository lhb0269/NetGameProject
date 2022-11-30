#pragma once
#include "Enemy.h"

class Tower : public  Enemy
{
	LONG moveRay;
	POINT axis;

	void drawBody(HDC);
	BOOL isSpawn;
	BOOL isTargetOn;

public:
	Tower();
	~Tower();
	void draw(HDC) override;
	void move(const POINT&) override;
	BOOL beAttacked(const LKM::Shape* hitBox) override;
	void bang(const POINT& player, BulletManager& bm, bool type);
	virtual bool IsChildClass() { return true; }
};

