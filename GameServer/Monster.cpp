#include "global.h"
#include "Monster.h"

#define CNT_START 180

LONG HeadedMob::speed = 4;
LONG HeadlessMob::speed = 5;
LONG Bomber::speed = 5;
LONG Slug::speed = 4;

void HeadedMob::drawBody(HDC hdc)
{
	Polygon(hdc, body.ptls, body.nPt);
}


HeadedMob::HeadedMob() : body(5) {
	pos = { NULL };
	isSpawn = false;
}

HeadedMob::~HeadedMob()
{
}


//void HeadedMob::draw(HDC hdc) {
//	HPEN oldpen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//	HBRUSH old = (HBRUSH)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//	drawBody(hdc);
//	LONG temp = size;
//	size = size * 2;
//	drawProtector(hdc);
//	size = temp;
//	SelectObject(hdc, oldpen);
//	SelectObject(hdc, old);
//}

BOOL HeadedMob::beAttacked(const LKM::Shape* hitBox)
{
	return body.Collide2Shape(hitBox);
}

BOOL HeadedMob::goOut()
{
	return false;
}

//void HeadedMob::bang(const POINT & playerPos, BulletManager & bm, bool type)
//{
//	bm.addBullet(body.ptls[2], playerPos, type);
//}

void HeadedMob::move(const POINT& p)
{
	POINT directoin = { p.x - pos.x, pos.y - p.y };
	angle = (FLOAT)atan2(directoin.y, directoin.x);
	velocity = { (cos(angle) * speed), - (sin(angle) * speed) };
	translocate(10);

	RECT bodyRect = getBody();
	body.ptls[0] = { bodyRect.left, bodyRect.top };
	body.ptls[1] = { bodyRect.right, bodyRect.top };
	body.ptls[2] = { pos.x + size * 2, pos.y };
	body.ptls[3] = { bodyRect.right, bodyRect.bottom };
	body.ptls[4] = { bodyRect.left, bodyRect.bottom };
	for (int i = 0; i < body.nPt; ++i) {
		body.ptls[i] = LKM::rotatePoint(pos, body.ptls[i], angle);
	}
}


//


void HeadlessMob::drawBody(HDC hdc)
{
	Polygon(hdc, body.ptls, body.nPt);
}

HeadlessMob::HeadlessMob() : body(5)
{
	isTargetOn = false;
	pos = { NULL };
	size = 30;
	isSpawn = false;
	rotate = 0;
}

HeadlessMob::~HeadlessMob() {
}

//void HeadlessMob::draw(HDC hdc)
//{
//	HPEN oldpen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//	HBRUSH old = (HBRUSH)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//	drawBody(hdc);
//	drawProtector(hdc);
//	SelectObject(hdc, oldpen);
//	SelectObject(hdc, old);
//}

void HeadlessMob::move(const POINT & p)
{
	if (isTargetOn) {
		velocity = { (cos(angle) * speed), -(sin(angle) * speed) };
		translocate(10);
	}
	else {
		POINT directoin = { p.x - pos.x, pos.y - p.y };
		angle = (FLOAT)atan2(directoin.y, directoin.x);
		velocity = { (cos(angle) * speed), -(sin(angle) * speed) };
		translocate(10);
		isTargetOn = true;
	}
	LKM::shapeRegularPlg(size, pos, body);
	for (int i = 0; i < body.nPt; ++i) {
		body.ptls[i] = LKM::rotatePoint(pos, body.ptls[i], angle + LKM::getRadian(rotate));
	}
	rotate += 3;
}

BOOL HeadlessMob::beAttacked(const LKM::Shape* hitBox)
{
	return body.Collide2Shape(hitBox);
}


BOOL HeadlessMob::goOut()
{
	isTargetOn = false;
	return false;
}

//void HeadlessMob::bang(const POINT & playerPos, BulletManager & bm, bool type)
//{
//	for (int i = 0; i < body.nPt; ++i) {
//		bm.addBullet(pos, body.ptls[i], type);
//	}
//}


//


//void Bomber::drawCore(HDC hdc)
//{
//	RECT range = getBody();
//	Ellipse(hdc, range.left, range.top, range.right, range.bottom);
//}
//
//void Bomber::drawSencer(HDC hdc)
//{
//	for (int i = 0; i < 4; ++i) {
//		POINT temp = { pos.x + size * 2, pos.y };
//		temp = LKM::rotatePoint(pos, temp, LKM::getRadian(90) * i);
//		RECT rect = { temp.x - 2, temp.y - 2, temp.x + 2, temp.y + 2 };
//		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
//	}
//}

const int rangeSize = 200;
//void Bomber::drawRegion(HDC hdc)
//{
//	HPEN hpen = CreatePen(PS_DOT, 1, RGB(0xff, 0x22, 0x22));
//	HPEN oldp = (HPEN)SelectObject(hdc, hpen);
//	HBRUSH oldb = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
//
//	RECT range = { pos.x - rangeSize , pos.y - rangeSize, pos.x + rangeSize , pos.y + rangeSize };
//	Ellipse(hdc, range.left, range.top, range.right, range.bottom);
//
//	SelectObject(hdc, oldb);
//	SelectObject(hdc, oldp);
//	DeleteObject(hpen);
//}

Bomber::Bomber()
{
	isCon = false;
	isBoom = false;
}

Bomber::~Bomber()
{
}

//void Bomber::draw(HDC hdc)
//{
//	COLORREF color = isBoom ? RGB(0xff, 0x22, 0x22) : RGB(0, 0, 0);
//	static int oSize = size;
//	size = isCon ? oSize + 5 * sin(LKM::getRadian(CNT_START - boomCnt)) : size;
//	HBRUSH newb = CreateSolidBrush(color);
//	HBRUSH oldb = (HBRUSH)SelectObject(hdc, newb);
//	HPEN newp = CreatePen(PS_SOLID, 1, color);
//	HPEN oldp = (HPEN)SelectObject(hdc, newp);
//	drawRegion(hdc);
//	drawSencer(hdc);
//	drawCore(hdc);
//	SelectObject(hdc, oldb);
//	SelectObject(hdc, oldp);
//	DeleteObject(newb);
//	DeleteObject(newp);
//}

void Bomber::move(const POINT & p)
{
	if (!isCon) {
		POINT directoin = { p.x - pos.x, pos.y - p.y };
		angle = (FLOAT)atan2(directoin.y, directoin.x);
		velocity = { (cos(angle) * speed), -(sin(angle) * speed) };
		translocate(-1);
		FLOAT len = LKM::getLength(LKM::exchange(directoin));
		if (len < rangeSize) {
			isCon = true;
		}
		boomCnt = CNT_START;
	}
	else {
		boomCnt -= 5;
		if (boomCnt == 0) {
			isBoom = true;
		}
	}
}

BOOL Bomber::goOut()
{
	return false;
}

BOOL Bomber::isShoot()
{
	return false;
}

LONG Bomber::getBombRange()
{
	return isBoom ? rangeSize : size;
}


Slug::Slug() : body(5)
{
	pos = { NULL };
	isSpawn = false;
}


Slug::~Slug()
{
}

//void Slug::draw(HDC hdc)
//{
//	HPEN oldpen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//	HBRUSH old = (HBRUSH)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//	Polygon(hdc, body.ptls, body.nPt);
//	LONG temp = size;
//	size = size * 2;
//	drawProtector(hdc);
//	size = temp;
//	SelectObject(hdc, oldpen);
//	SelectObject(hdc, old);
//}

void Slug::move(const POINT & target)
{
	POINT directoin = { target.x - pos.x, pos.y - target.y };
	angle = (FLOAT)atan2(directoin.y, directoin.x);
	velocity = { (cos(angle) * speed), -(sin(angle) * speed) };
	translocate(10);

	RECT bodyRect = getBody();
	body.ptls[0] = { bodyRect.left, bodyRect.top };
	body.ptls[1] = { bodyRect.right, bodyRect.top + 5};
	body.ptls[4] = { pos.x - size * 2, pos.y };
	body.ptls[2] = { bodyRect.right, bodyRect.bottom - 5};
	body.ptls[3] = { bodyRect.left, bodyRect.bottom };
	for (int i = 0; i < body.nPt; ++i) {
		body.ptls[i] = LKM::rotatePoint(pos, body.ptls[i], angle);
	}
}

BOOL Slug::beAttacked(const LKM::Shape * hitBox)
{
	return body.Collide2Shape(hitBox);
}


//void Slug::bang(const POINT & playerPos, BulletManager & bm, bool type)
//{
//	POINT temp = { (body.ptls[1].x + body.ptls[2].x) / 2, (body.ptls[1].y + body.ptls[2].y) / 2 };
//	bm.addBullet(temp, LKM::rotatePoint(pos, playerPos, LKM::getRadian(30)), false);
//	bm.addBullet(temp, playerPos, false);
//	bm.addBullet(temp, LKM::rotatePoint(pos, playerPos, LKM::getRadian(-30)), false);
//}
