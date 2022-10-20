#pragma once
#include "Queue.h"
#include "Object.h"

enum Effect_Type{
	Create, Destroy, Break, Particle_Normal, Particle_Super, BTOOM, BOSS_DAMAGED, TYPE_NUM
};

class Effect {
protected:
	bool bEnd;
	POINT pos;
	int stack;
public:
	Effect();
	~Effect();
	POINT getPos();
	void setPos(POINT point);
	virtual void draw(HDC);
	virtual void update();
	bool isEnd();
};

class DestoyEffect : public Effect {
	FLOAT rad[5];
	LONG scale;
public:
	DestoyEffect();
	~DestoyEffect();
	void draw(HDC) override;
	void update() override;
};

class SpawnEffect : public Effect {
	LONG scales[5];
public:
	SpawnEffect();
	~SpawnEffect();
	void draw(HDC) override;
	void update() override;
};

class BreakShieldEffect : public Effect {
	LONG scale;
public:
	BreakShieldEffect();
	~BreakShieldEffect();
	void draw(HDC) override;
	void update() override;
};

class Particle : public Effect {
	const int size;
	FLOAT rad[3];
	COLORREF color;
public:
	Particle();
	Particle(COLORREF);
	~Particle();
	void setColor(COLORREF);
	void draw(HDC) override;
	void update() override;
};

class BtoomEffect : public Effect {
	POINT point[5];
	LONG scale[5];
public:
	BtoomEffect();
	~BtoomEffect();
	void draw(HDC) override;
	void update() override;
};

class FlameEffect : public Effect {
	POINT dot[50];
	LONG size;
	LONG ray;
	COLORREF color;
public:
	FlameEffect();
	~FlameEffect();
	void draw(HDC) override;
	void update() override;
	void setSize(LONG);
};

class EffectManager
{
	Queue<Effect> effects;
	int effectNum;
public:
	EffectManager();
	~EffectManager();
	void draw(HDC);
	void add(POINT, char);
};
