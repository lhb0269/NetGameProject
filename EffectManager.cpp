#include "stdafx.h"
#include "EffectManager.h"
#include "Queue.cpp"

extern std::mt19937_64 re;
std::uniform_int_distribution<LONG> uid;

EffectManager::EffectManager()
{
	effectNum = 0;
}


EffectManager::~EffectManager()
{
}

void EffectManager::draw(HDC hdc)
{
	Effect* cur;
	int time = effectNum;
	for (int i = 0; i < time; ++i) {
		cur = effects.get();
		cur->update();
		cur->draw(hdc);
		if (cur->isEnd()) {
			delete cur;
			effectNum--;
		}
		else {
			effects.put(cur);
		}
	}
}

void EffectManager::add(POINT pos, char typeSwitch)
{
	Effect* now = nullptr;
	switch (typeSwitch) {
	case Create:
		now = new SpawnEffect;
		break;
	case Destroy:
		now = new DestoyEffect;
		break;
	case Break:
		now = new Particle;
		dynamic_cast<Particle*>(now)->setColor(LKM::BlendColor(RGB(0xff, 0xff, 0xff), RGB(0xee, 0xe6, 0xcc), 0.9f));
		now->setPos(pos);
		effects.put(now);
		effectNum++;
		now = new BreakShieldEffect;
		break;
	case Particle_Normal:
		now = new Particle(RGB(255, 128, 64));
		break;
	case Particle_Super:
		now = new Particle(RGB(128, 0, 255));
		break;
	case BTOOM:
		now = new BtoomEffect;
		break;
	case BOSS_DAMAGED:
		now = new Particle(RGB(0xff, 0xff, 0xff));
		now->setPos(pos);
		effects.put(now);
		effectNum++;
		now = new Particle(RGB(0x0, 0x0, 0x0));
		break;
	default:
		return;
	}
	now->setPos(pos);
	effects.put(now);
	effectNum++;

}

//

Effect::Effect()
{
	stack = -1;
	bEnd = false;
}

Effect::~Effect()
{
}

POINT Effect::getPos()
{
	return pos;
}

void Effect::setPos(POINT point)
{
	pos = point;
}

void Effect::draw(HDC)
{
}

void Effect::update()
{
	bEnd = true;
}

bool Effect::isEnd()
{
	return bEnd;
}

//

DestoyEffect::DestoyEffect()
{
}

DestoyEffect::~DestoyEffect()
{
}

void DestoyEffect::draw(HDC hdc)
{
	RECT frame = LKM::makeRect(pos, scale);
	HPEN hpen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	HPEN old = (HPEN)SelectObject(hdc, hpen);
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, frame.left, frame.top, frame.right, frame.bottom);
	SelectObject(hdc, old);
	DeleteObject(hpen);
	SelectObject(hdc, oldbrush);

	const int size = 5;
	HPEN oldpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
	oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
	LONG len = stack * size;
	for (auto a : rad) {
		POINT p = { pos.x + LONG(cos(a) * len), pos.y + LONG(sin(a) * len) };
		RECT frame = LKM::makeRect(p, size);
		Rectangle(hdc, frame.left, frame.top, frame.right, frame.bottom);
	}
	DeleteObject(hbrush);
	SelectObject(hdc, oldbrush);
	SelectObject(hdc, oldpen);
}

void DestoyEffect::update()
{
	const int endScale = 30;
	if (stack < 0) {
		scale = 0;
		stack = 0;
		for (auto& a : rad) {
			a = LKM::getRadian(uid(re));
		}
	}
	else {
		stack++;
		scale = stack * 3;
		if (scale > endScale) {
			bEnd = true;
		}
	}
}

//

SpawnEffect::SpawnEffect()
{
	for (auto &scale : scales) {
		scale = 0;
	}
}

SpawnEffect::~SpawnEffect()
{
}

void SpawnEffect::draw(HDC hdc)
{

	HPEN hpen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	HPEN old = (HPEN)SelectObject(hdc, hpen);
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	for (auto scale : scales) {
		RECT frame = LKM::makeRect(pos, scale);
		Ellipse(hdc, frame.left, frame.top, frame.right, frame.bottom);
	}
	SelectObject(hdc, old);
	DeleteObject(hpen);
	SelectObject(hdc, oldbrush);

}

void SpawnEffect::update()
{
	const int endScale = 30;
	const int cut = 4;
	if (stack < 0) {
		stack = 0;
	}
	else {
		stack++;

		bool endcheck = true;

		for (auto &scale : scales) {
			if (scale < endScale) {
				scale += endScale / cut;
				endcheck = false;
				break;
			}
		}

		if (endcheck) {
			bEnd = true;
		}
	}
}

BreakShieldEffect::BreakShieldEffect()
{
	scale = 0;
}

BreakShieldEffect::~BreakShieldEffect()
{
}

void BreakShieldEffect::draw(HDC hdc)
{
	LONG orbitRay = scale;
	RECT shell = { pos.x - orbitRay, pos.y - orbitRay, pos.x + orbitRay, pos.y + orbitRay };
	HPEN pen = CreatePen(PS_SOLID, 5, LKM::BlendColor(RGB(0xff, 0xff, 0xff), RGB(0xee, 0xe6, 0xcc), 0.9f));
	HPEN  oldpen = (HPEN)SelectObject(hdc, pen);
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, shell.left, shell.top, shell.right, shell.bottom);
	SelectObject(hdc, oldpen);
	SelectObject(hdc, oldbrush);
	DeleteObject(pen);
}

void BreakShieldEffect::update()
{
	const LONG endScale = 60;
	if (stack < 0) {
		stack = 0;
	}
	else {
		stack++;
		scale += endScale * sin(LKM::getRadian(stack * 5));
		if (scale > endScale) {
			bEnd = true;
		}
	}
}

Particle::Particle() : size(5)
{
	for (auto& a : rad) {
		a = { NULL };
	}
}

Particle::Particle(COLORREF color) : size(5), color(color)
{
}

Particle::~Particle()
{
}

void Particle::setColor(COLORREF iCl)
{
	color = iCl;
}

void Particle::draw(HDC hdc)
{
	HPEN oldpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	HBRUSH hbrush = CreateSolidBrush(color);
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
	LONG len = stack * size;
	for (auto a : rad) {
		POINT p = { pos.x + LONG(cos(a) * len), pos.y + LONG(sin(a) * len) };
		RECT frame = LKM::makeRect(p, size);
		Ellipse(hdc, frame.left, frame.top, frame.right, frame.bottom);
	}
	DeleteObject(hbrush);
	SelectObject(hdc, oldbrush);
	SelectObject(hdc, oldpen);
}

void Particle::update()
{
	if (stack < 0) {
		stack = 0;
		for (auto& a : rad) {
			a = LKM::getRadian(uid(re));
		}
	}
	else {
		stack++;
		if (stack > 15) {
			bEnd = true;
		}
	}
}

BtoomEffect::BtoomEffect()
{
	for (auto& a : point) {
		FLOAT rad = LKM::getRadian(uid(re));
		LONG len = 1 + stack + uid(re) % 5;
		a.x = pos.x + cos(rad) * len;
		a.y = pos.y + sin(rad) * len;
	}
	for (auto& a : scale) {
		a = 10 + uid(re) % 15;
	}
}

BtoomEffect::~BtoomEffect()
{
}


void BtoomEffect::draw(HDC hdc)
{
	HPEN oldpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
	int i = 0;
	for (auto p : point) {
		RECT frame = LKM::makeRect(p, scale[i]);
		Ellipse(hdc, frame.left, frame.top, frame.right, frame.bottom);
		i++;
	}
	DeleteObject(hbrush);
	SelectObject(hdc, oldbrush);
	SelectObject(hdc, oldpen);
}

void BtoomEffect::update()
{
	const int cut = 15;

	if (stack < 0) {
		stack = 0;
	}
	else {
		stack++;
		LONG len = 20 + stack * 5;
		FLOAT rad = LKM::getRadian(uid(re));
		point[stack % 5].x = pos.x + cos(rad) * len;
		point[stack % 5].y = pos.y + sin(rad) * len;
		scale[stack % 5] = 10 + uid(re) % 50;

		for (auto& a : scale) {
			a -= 4;
		}

		if (stack > cut) {
			bEnd = true;
		}
	}
}

FlameEffect::FlameEffect()
{
	size = 100;
}

FlameEffect::~FlameEffect()
{
}

void FlameEffect::draw(HDC hdc)
{
	HPEN oldpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

	static int j = 0;
	const int t = 10;
	for (int i = j; i < j + t; ++i) {
		FLOAT rad = LKM::getRadian(uid(re));
		dot[i] = { ray * (LONG)cos(rad), ray * (LONG)sin(rad) };
	}
	j += t;
	DeleteObject(hbrush);
	SelectObject(hdc, oldbrush);
	SelectObject(hdc, oldpen);
}

void FlameEffect::update()
{
	const int cut = 15;
	if (stack < 0) {
		stack = 0;
		ray = size - cut / 2 + stack;
		for (auto& p : dot) {
			FLOAT rad = LKM::getRadian(uid(re));
			p = { ray * (LONG)cos(rad), ray * (LONG)sin(rad) };
		}
	}
	else {
		stack++;

		ray = size - cut / 2 + stack;

		if (stack > cut) {
			bEnd = true;
		}
	}
}

void FlameEffect::setSize(LONG len)
{
	size = len;
}
