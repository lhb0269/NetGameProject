#pragma once
#include "LKM_Physics.h"
#include "Object.h"
#include "BulletManager.h"


enum STATE {
	none, be_spawn, be_breaken, be_destroyed
};

class Enemy : public RealObject
{ 
	char spawnStack;
	int type = 0;
	int shape = 0;
protected:
	LONG size;
	RECT getBody();
	BOOL isSpawn;
	POINT previous_pos;
	int	 state;
	bool bProtected;

	virtual void drawProtector(HDC hdc);
public:
	Enemy();
	~Enemy();
	virtual void draw(HDC);
	virtual void move(const POINT&);
	virtual	BOOL beAttacked(const LKM::Shape* hitBox);
	virtual BOOL start(const POINT& spawnPoint);
	virtual BOOL goOut();
	virtual BOOL isShoot();
	virtual void bang(const POINT& playerPos, BulletManager& bm, bool type);
	virtual void protectOnOff(bool);
	LONG getSize();
	bool isSpawned();
	void spawnSignal();
	bool isProtect();
	void setShape(int num);
	void SetState(int n) { state = n; }
	int GetState() const { return state; };
	void SetPrePos(POINT pos) { previous_pos = pos; }
	POINT GetPrePos() const { return previous_pos; };
};

