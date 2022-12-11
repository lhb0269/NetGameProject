#include "global.h"
#include "BulletManager.h"

FLOAT Bullet::maxSpeed = 7;
LONG Bullet::size = 15;

COLORREF Bullet::getColor()
{
	COLORREF color;
	switch (type)
	{
	case NORMAL:
		color = RGB(255, 128, 64);
		break;
	case SUPER:
		color = RGB(128, 0, 255);
		break;
	default:
		color = RGB(0, 128, 64);
		break;
	}
	return color;
}

Bullet::Bullet(POINT start, POINT target, int Type)
{
	visible = true;
	type = Type;
	pos = start;
	POINTFLOAT temp = { (FLOAT)target.x - pos.x, (FLOAT)target.y - pos.y };
	FLOAT len = LKM::getLength(temp);
	velocity = { temp.x / len * maxSpeed, temp.y / len * maxSpeed };
}

Bullet::~Bullet()
{
}

RECT Bullet::getRect()
{
	return { pos.x - size, pos.y - size, pos.x + size, pos.y + size };
}

LONG Bullet::getSize()
{
	return size;
}

bool Bullet::getType()

{
	return type == NORMAL;
}

int Bullet::igetType()
{
	return type;
}

void Bullet::draw(HDC hdc, COLORREF mapColor)
{
	COLORREF color = getColor();
	COLORREF line = LKM::BlendColor(color, mapColor, 0.5);
	RECT rect = getRect();

	HBRUSH brush = CreateSolidBrush(color);
	HPEN pen = CreatePen(PS_SOLID, 1, line);
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);
	HBRUSH old = (HBRUSH)SelectObject(hdc, brush);

	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

	SelectObject(hdc, oldpen);
	SelectObject(hdc, old);
	DeleteObject(brush);
	DeleteObject(pen);
}

void Bullet::move()
{
	translocate( maxSpeed );
}

bool Bullet::collideTo(const LKM::Shape* sword)
{
	LKM::Shape temp(6);
	LKM::shapeHexagon(size, pos, temp);
	if (temp.Collide2Shape(sword)) {
		return true;
	}
	else
		return false;
}
