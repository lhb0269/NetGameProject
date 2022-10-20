#pragma once
#include "Enemy.h"

class HeadedMob : public Enemy
{
	static LONG speed;
	LKM::Shape body;
	void drawBody(HDC);
public:
	HeadedMob();
	~HeadedMob();
	void draw(HDC) override;
	void move(const POINT&) override;
	BOOL beAttacked(const LKM::Shape* hitBox) override;
	BOOL goOut() override;
	void bang(const POINT& playerPos, BulletManager& bm, bool type) override;
};

class HeadlessMob : public Enemy
{
	static LONG speed;
	LKM::Shape body;
	void drawBody(HDC);
	BOOL isTargetOn;
	short rotate;

public:
	HeadlessMob();
	~HeadlessMob();
	void draw(HDC) override;
	void move(const POINT&) override;
	BOOL beAttacked(const LKM::Shape* hitBox) override;
	BOOL goOut() override;
	void bang(const POINT& playerPos, BulletManager& bm, bool type) override;
};

class Bomber : public Enemy
{
	static LONG speed;
	bool isCon;
	bool isBoom;
	int boomCnt;

	void drawCore(HDC);
	void drawSencer(HDC);
	void drawRegion(HDC);
public:
	Bomber();
	~Bomber();
	void draw(HDC) override;
	void move(const POINT&) override;
	BOOL goOut() override;
	BOOL isShoot() override;
	LONG getBombRange();
};

class Slug :
	public Enemy
{
	static LONG speed;
	LKM::Shape body;

public:
	Slug();
	~Slug();
	void draw(HDC) override;
	void move(const POINT&) override;
	BOOL beAttacked(const LKM::Shape* hitBox) override;
	void bang(const POINT& playerPos, BulletManager& bm, bool type) override;
};



