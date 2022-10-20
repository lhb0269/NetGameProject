#pragma once
#include "Enemy.h"

struct Flame {
	LONG axisDistance;
	FLOAT axisRad;

	LONG endDistance;
	FLOAT endRad;

	bool bExist;

	Flame();
	~Flame();
};

class Sun :
	public Enemy
{
	static LONG speed;
	int stack;

	int hp;
	bool bDamaged;
	bool bShoot;

	Flame flames[12];
	int fNum;

	POINT transPosToAxis(POINT axis, FLOAT rad, LONG distance);
	POINT transPos(FLOAT rad, LONG distance);
	void drawBody(HDC);
	void drawLeg(HDC);
public:
	Sun();
	~Sun();
	void draw(HDC) override;
	void move(const POINT&) override;
	BOOL beAttacked(const LKM::Shape* hitBox) override;
	void bang(const POINT& playerPos, BulletManager& bm, bool type) override;
};

//struct Flame {
//	FLOAT rad;
//	POINT pos;
//	bool bExist;
//	Flame();
//	~Flame();
//	void draw(HDC hdc);
//	void beAttacked(LKM::Shape hitBox);
//};
//
//class Sun : public Enemy {
//	static LONG speed;
//	int hp;
//	bool bDamaged;
//
//	void drawCore();
//	void drawFlame();
//
//public:
//	Sun();
//	~Sun();
//	void draw(HDC) override;
//	void move(const POINT&) override;
//	BOOL beAttacked(const LKM::Shape* hitBox) override;
//	void bang(const POINT& playerPos, BulletManager& bm, bool type) override;
//};