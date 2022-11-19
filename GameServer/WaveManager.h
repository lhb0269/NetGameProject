#pragma once
#include "Queue.h"

class Monster {
public:
	POINT start;
	int type;
	bool protect;
	Monster();
	~Monster();
};

class WaveManager
{
	int level;
	int score;
	int timeStack;
	bool spawnNow;
	int spawnNum;

	Queue<Monster> buffer;
	short mobptr;

	POINT axis;
	LONG radius;

	POINT getRanPos();
	void wave1();
	void wave2();
	void wave3();
	void wave5();
	void wave7();
	void wave11();
	void wave13();
	void wave17();
	void wave19();
	void wave23();
	

	void levelUp();
public:
	WaveManager();
	~WaveManager();
	int getLevel();
	bool isSpawn();
	void addScore(int add);
	void update();
	Monster* getMob();
	void newMob();
	void setLevel();
};

