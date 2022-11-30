#include "stdafx.h"
#include "Block.h"

LONG Block::size = BLOCK_SIZE;

Block::Block(POINT point, int type)
{
	pos = point;
	breakable = type == 0;
	threarening = type > 1 ? true : false;
}


Block::~Block()
{
}

BOOL Block::beTouched(const LKM::Shape * hitBox)
{
	LKM::Shape temp(4);
	RECT rect = getRect();
	LKM::transRect2Shape(&rect, &temp);
	return temp.Collide2Shape(hitBox);
}

void Block::draw(HDC hdc)
{	
	COLORREF color;
	if (!breakable) {
		if (threarening) {
			color = RGB(0xff, 0x23, 0x23);
		}
		else {
			color = RGB(0xff, 0xff, 0xff);
		}
	}
	else {
		color = RGB(0, 0, 0);
	}
	HBRUSH newb = CreateSolidBrush(color);
	HBRUSH oldb = (HBRUSH)SelectObject(hdc, newb);
	HPEN newp = CreatePen(PS_SOLID, 1, color);
	HPEN oldp = (HPEN)SelectObject(hdc, newp);
	
	RECT rect = getRect();
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	SelectObject(hdc, oldb);
	SelectObject(hdc, oldp);
	DeleteObject(newb);
	DeleteObject(newp);
	DeleteObject(oldb);
	DeleteObject(oldp);
}

RECT Block::getRect() {
	return { pos.x - size, pos.y - size, pos.x + size, pos.y + size };
}

BOOL Block::isBreakable()
{
	return breakable;
}
