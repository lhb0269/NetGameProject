#include "global.h"


void Tower::drawBody(HDC hdc)
{
	RECT rect = getBody();
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

Tower::Tower()
{
	isSpawn = false;
}


Tower::~Tower()
{
}

//void Tower::draw(HDC hdc)
//{
//	HPEN hepn = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
//	HBRUSH hbrush = (HBRUSH)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//	drawBody(hdc);
//	drawProtector(hdc);
//	SelectObject(hdc, hepn);
//	SelectObject(hdc, hbrush);
//}

void Tower::move(const POINT & player)
{
}

BOOL Tower::beAttacked(const LKM::Shape* hitBox)
{
	LKM::Shape temp(6);
	LKM::shapeHexagon(size, pos, temp);
	return temp.Collide2Shape(hitBox);
}

//void Tower::bang(const POINT & player, BulletManager & bm, bool type)
//{
//	LKM::Shape gun(6);
//	LKM::shapeHexagon(size, pos, gun);
//	for (int i = 0; i < gun.nPt; ++i) {
//		bm.addBullet(pos, gun.ptls[i], type);
//	}
//}
