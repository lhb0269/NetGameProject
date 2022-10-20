#include "stdafx.h"
#include "Boss.h"
#include "EffectManager.h"

LONG Sun::speed = 5;

extern EffectManager effectMng;

POINT Sun::transPos(FLOAT rad, LONG distance)
{
	return { pos.x + LONG(cos(angle + rad) * distance), pos.y - LONG(sin(angle + rad) * distance )};
}

POINT Sun::transPosToAxis(POINT axis, FLOAT rad, LONG distance)
{
	return { pos.x + LONG(cos(angle + rad) * distance), pos.y - LONG(sin(angle + rad) * distance) };
}

void Sun::drawBody(HDC hdc)
{
	LONG len = size / 2 - 20;
	POINT axis;
	axis.x = pos.x + len * cos(angle);
	axis.y = pos.y - len * sin(angle);
	POINT tail;
	tail.x = pos.x - (size - 10) * cos(angle);
	tail.y = pos.y + (size - 10) * sin(angle);

	//FLOAT lenLine = sqrt(size * size * 3 / 4);
	//POINT start;
	//start.x = axis.x + lenLine * cos(angle + LKM::getRadian(90));
	//start.y = axis.y - lenLine * sin(angle + LKM::getRadian(90));
	//POINT end;
	//end.x = axis.x + lenLine * cos(angle - LKM::getRadian(90));
	//end.y = axis.y - lenLine * sin(angle - LKM::getRadian(90));

	HPEN hpen = CreatePen(PS_SOLID, 1, RGB(0x00, 0x00, 0x00));
	HPEN oldp = (HPEN)SelectObject(hdc, hpen);
	HBRUSH brush = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	HBRUSH oldb = (HBRUSH)SelectObject(hdc, brush);
	RECT range = { pos.x - size , pos.y - size, pos.x + size, pos.y + size };
	Ellipse(hdc, range.left, range.top, range.right, range.bottom);
	SelectObject(hdc, oldb);
	SelectObject(hdc, oldp);
	DeleteObject(hpen);
	DeleteObject(brush);


	//hpen = CreatePen(PS_SOLID, 1, RGB(0x0, 0x0, 0x0));
	//oldp = (HPEN)SelectObject(hdc, hpen);
	//brush = CreateSolidBrush(RGB(0x0, 0x22, 0x22));
	//oldb = (HBRUSH)SelectObject(hdc, brush);

	//MoveToEx(hdc, axis.x, axis.y, NULL);
	//LineTo(hdc, tail.x, tail.y);


	//MoveToEx(hdc, start.x, start.y, NULL);
	//LineTo(hdc, end.x, end.y);

	//Chord(hdc, range.left, range.top, range.right, range.bottom, end.x, end.y, start.x, start.y);

	//SelectObject(hdc, oldb);
	//SelectObject(hdc, oldp);
	//DeleteObject(hpen);
	//DeleteObject(brush);
}

void Sun::drawLeg(HDC hdc)
{
	HPEN hpen = CreatePen(PS_SOLID, 10, RGB(0x00, 0x0, 0x0));
	HPEN oldp = (HPEN)SelectObject(hdc, hpen);

	for (auto& leg : flames) {
		if (leg.bExist) {
			POINT axis = { pos.x + cos(angle + leg.axisRad) * leg.axisDistance, pos.y - sin(angle + leg.axisRad) * leg.axisDistance };
			POINT end = { axis.x + cos(angle + leg.endRad) * leg.endDistance, axis.y - sin(angle + leg.endRad) * leg.endDistance };
			MoveToEx(hdc, axis.x, axis.y, NULL);
			LineTo(hdc, end.x, end.y);
		}
	}

	SelectObject(hdc, oldp);
	DeleteObject(hpen);
}

Sun::Sun() {
	stack = -1;
	bDamaged = true;
	bShoot = false;
	hp = 3;
	size = 100;
	angle = 0;
	pos = { NULL };

	fNum = 12;
	int i = 0;
	for (auto& flame : flames) {
		flame.axisRad = LKM::getRadian(360/ fNum * i);
		flame.endRad = flame.axisRad;
		flame.axisDistance = size * 1.1;
		flame.endDistance = 30;
		flame.bExist = true;
		i++;
	}
}


Sun::~Sun()
{
}

void Sun::draw(HDC hdc)
{
	drawLeg(hdc);
	drawBody(hdc);
}

void Sun::move(const POINT & target)
{
	if (stack < 0) {
		POINT target = { WHOLE_MAP / 2, WHOLE_MAP / 2 - MAP_RAY };
		angle = atan2(pos.y - target.y, target.x - pos.x);
		LONG len = LKM::getLength(LKM::exchange(target), LKM::exchange(pos));
		velocity = { cos(angle) * len, -sin(angle) * len };
		translocate(speed * 10);
		bShoot = false;
		if (len < 5) {
			stack = 0;
		}
	}
	else {
		stack++;
		POINT center = { WHOLE_MAP / 2, WHOLE_MAP / 2 };
		POINT ray = { WHOLE_MAP / 2, WHOLE_MAP / 2 + MAP_RAY };
		POINT target = LKM::rotatePoint(center, ray, LKM::getRadian(stack));
		angle = atan2(pos.y - target.y, target.x - pos.x);
		velocity = { cos(angle) * speed, -sin(angle) * speed };
		translocate(speed);
		bShoot = true;

		static int last;
		if (bDamaged) {
			last = stack;
		}
		else {
			if (stack - last > 200)
				bDamaged = true;
		}
	}
}

BOOL Sun::beAttacked(const LKM::Shape * hitBox)
{
	if (bDamaged) {
		int cnt = 0;
		for (auto& one : flames) {
			if (one.bExist) {
				POINT axis = transPos(one.axisRad, one.axisDistance);
				POINT end = transPosToAxis(axis, one.endRad, one.endDistance);
				if (LKM::collideS2L(hitBox, axis, end)) {
					one.bExist = false;
					bDamaged = false;
					fNum--;
					return false;
				}
				cnt++;
			}
		}
		if (cnt == 0) {
			LKM::Shape temp(10);
			LKM::shapeRegularPlg(size, pos, temp);
			if (temp.Collide2Shape(hitBox)) {
				hp--;
				if (hp <= 0) {
					return true;
				}
			}
		}
		return false;
	}else
		return false;
}

void Sun::bang(const POINT & playerPos, BulletManager & bm, bool type)
{
	if (bShoot) {
		for (auto& one : flames) {
			if (one.bExist) {
				POINT axis = transPos(one.axisRad, one.axisDistance);
				POINT end = transPosToAxis(axis, one.endRad, one.endDistance);
				bm.addBullet(axis, end, type);
			}
		}
	}
}

Flame::Flame()
{
}

Flame::~Flame()
{
}
