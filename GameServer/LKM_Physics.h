#pragma once
#include <Windows.h>

namespace LKM {
	struct Shape
	{
		POINT* ptls;
		int nPt;
		Shape(const int nPoint);
		~Shape();
		void translocate(POINT);
		BOOL Colide2Point(POINT point);
		BOOL Collide2Shape(const Shape* other);
		BOOL Colide2Line(POINT s, POINT e);
	};

	BOOL shapeHexagon(const LONG ray, const POINT center, Shape& shpae);
	BOOL shapeRegularPlg(const LONG ray, const POINT center, Shape& shpae);
	POINT rotatePoint(const POINT pos, const POINT dot, FLOAT rad);
	FLOAT getRadian(int angle);
	int getUnit(double);
	FLOAT decreaseReal(FLOAT, FLOAT);
	FLOAT getLength(POINTFLOAT, POINTFLOAT);
	FLOAT getLength(POINTFLOAT);
	POINT exchange(POINTFLOAT);
	POINTFLOAT exchange(POINT);
	BOOL isCollide(Shape&, Shape&);
	POINTFLOAT addForce(POINTFLOAT, POINTFLOAT);

	BOOL collideP2P(POINT, POINT, FLOAT); // P - P 간의 거리가 입력값과 같다면 트루
	BOOL collideR2C(RECT*, POINT, FLOAT); // RECT와 POINT가 
	BOOL collideS2L(const Shape* shape, POINT a, POINT b);

	COLORREF BlendColor(COLORREF, COLORREF, FLOAT);
	BOOL crossLine(POINT a, POINT b, POINT c, POINT d);
	RECT makeRect(POINT lt, POINT rb);
	RECT makeRect(POINT lt, LONG size);
	void transRect2Shape(RECT* rect, Shape* shape);
}