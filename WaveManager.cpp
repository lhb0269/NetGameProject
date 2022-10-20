#include "stdafx.h"
#include "WaveManager.h"
#include "LKM_Physics.h"
#include "Queue.cpp"


extern std::mt19937_64 re;
std::uniform_int_distribution<int> rAng(0, 360);

void WaveManager::levelUp()
{
	if (score > level * 20) {
		level++;
		score = 0;
	}
}

WaveManager::WaveManager()
{
	level = 1;
	score = 0;
	timeStack = 0;
	spawnNum = 0;
	mobptr = NULL;
	axis = { WHOLE_MAP / 2, WHOLE_MAP / 2 };
	radius = MAP_RAY;
}


WaveManager::~WaveManager()
{
}

int WaveManager::getLevel()
{
	return level;
}

bool WaveManager::isSpawn()
{
	return !buffer.isEmpty();
}

void WaveManager::addScore(int add)
{
	score += add;
	levelUp();
}

void WaveManager::update()
{
	timeStack++;
	static time_t last = NULL;
	time_t now = time(NULL);
	if (now - last >= 3) {
		score += level;
		last = now;
	}
	levelUp();
	newMob();
}
void WaveManager::setLevel()
{
	level = 1;
}
void WaveManager::newMob()
{
	wave1();
	wave2();
	wave3();
	wave5();
	wave7();
	wave11();
	wave13();
	wave17();
	wave19();
	wave23();
}

Monster* WaveManager::getMob()
{
	static Monster result;
	if (!buffer.isEmpty()) {
		Monster* cur = buffer.get();
		result = *cur;
		delete cur;
		return &result;
	}
	else
		return nullptr;
}

POINT WaveManager::getRanPos()
{
	float rad = LKM::getRadian(rAng(re));
	LONG l = (radius + 50);
	return { axis.x + (LONG)(cos(rad) * l), axis.y + (LONG)(sin(rad) * l) };
}

void WaveManager::wave1() // 
{
	static clock_t last = NULL;
	clock_t now = clock();

	if (now - last > 999) {
		last = now;
		for (int i = 0; i < 1; ++i) {
			Monster* mob = new Monster;
			mob->start = getRanPos();
			mob->type = HEADED;
			mob->protect = false;
			buffer.put(mob);
		}
	}
}

void WaveManager::wave2() // 
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 2;
	if (level % key == 0 && now - last > 3000) {
		last = now;
		{
			Monster* cur = new Monster;
			cur->protect = false;
			cur->start = getRanPos();
			cur->type = SLUG;
			buffer.put(cur);
		}
		{
			Monster* cur = new Monster;
			cur->protect = false;
			cur->start = getRanPos();
			cur->type = HEADLESS;
			buffer.put(cur);
		}
	}
}

void WaveManager::wave3()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 3;
	if (level % key == 0 && now - last > 5000) {
		last = now;
		for (int i = 0; i < 1; ++i) {
			Monster* cur = new Monster;
			cur->protect = true;
			cur->start = getRanPos();
			cur->type = HEADLESS;
			buffer.put(cur);
		}
	}
}

void WaveManager::wave5()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 5;
	if (level % key == 0 && now - last > 10000) {
		last = now;
		for (int i = 0; i < 1; ++i) {
			Monster* cur = new Monster;
			cur->protect = false;
			cur->start = getRanPos();
			cur->type = TOWER;
			buffer.put(cur);
		}
	}
}

void WaveManager::wave7()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 7;
	if (level % key == 0 && now - last > 6000) {
		last = now;
		
		POINT pos = getRanPos();
		Monster* cur = new Monster;
		cur->protect = true;
		cur->start = pos;
		cur->type = TOWER;
		buffer.put(cur);

		pos = getRanPos();
		cur = new Monster;
		cur->protect = false;
		cur->start = pos;
		cur->type = TOWER;
		buffer.put(cur);
	}
}

void WaveManager::wave11()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 11;
	if (level % key == 0 && now - last > 6000) {
		last = now;

		POINT pos = getRanPos();
		Monster* cur = new Monster;
		cur->protect = true;
		cur->start = pos;
		cur->type = SLUG;
		buffer.put(cur);

		pos = getRanPos();
		cur = new Monster;
		cur->protect = false;
		cur->start = pos;
		cur->type = HEADLESS;
		buffer.put(cur);
	}
}

void WaveManager::wave13()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 13;
	if (level % key == 0 && now - last > 8000) {
		last = now;

		Monster* cur = new Monster;
		cur->protect = false;
		cur->start = getRanPos();
		cur->type = TOWER;
		buffer.put(cur);

		cur = new Monster;
		cur->protect = true;
		cur->start = getRanPos();
		cur->type = HEADLESS;
		buffer.put(cur);

		cur = new Monster;
		cur->protect = true;
		cur->start = getRanPos();
		cur->type = BOMBER;
		buffer.put(cur);
	}
}

void WaveManager::wave17()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 17;
	if (level % key == 0 && now - last > 8000) {
		last = now;
	
		for (int i = 0; i < 2; ++i) {
			Monster* cur = new Monster;
			cur->protect = false;
			cur->start = getRanPos();
			cur->type =SLUG;
			buffer.put(cur);
		}
		{
			Monster* cur = new Monster;
			cur->protect = false;
			cur->start = getRanPos();
			cur->type = HEADED;
			buffer.put(cur);
		}
	}
}

void WaveManager::wave19()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 19;
	if (level % key == 0 && now - last > 9999) {
		last = now;
		LKM::Shape plg(10);
		LKM::shapeRegularPlg(400, axis, plg);
		for (int i = 0; i < plg.nPt; ++i) {
			Monster* cur = new Monster;
			cur->protect = false;
			cur->start = plg.ptls[i];
			cur->type = SLUG;
			buffer.put(cur);
		}
		
	}
}

void WaveManager::wave23()
{
	static clock_t last = NULL;
	clock_t now = clock();

	const int key = 23;
	if (level % key == 0 && now - last > 99999) {
		last = now;
		Monster* cur = new Monster;
		cur->protect = true;
		cur->start = getRanPos();
		cur->type = SUN;
		buffer.put(cur);
	}
}

Monster::Monster()
{
	start = { NULL };
	type = NULL;
	protect = NULL;
}

Monster::~Monster()
{
}
