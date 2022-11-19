#include "global.h"
#include "LKM_Physics.h"


BOOL LKM::shapeHexagon(const LONG ray, const POINT pos, Shape & shape )
{
	if (shape.nPt != 6) {
		return false;
	}
	else {
		for (int i = 0; i < shape.nPt; ++i) {
			shape.ptls[i] = { pos.x + (LONG)(ray * cos(LKM::getRadian(90 - i * 60))), pos.y + (LONG)(ray* sin(LKM::getRadian(90 - i * 60))) };
		}
		return true;
	}
}

BOOL LKM::shapeRegularPlg(const LONG ray, const POINT pos, Shape & shape)
{
	if (shape.nPt < 3) {
		if (shape.nPt == 2) {
			shape.ptls[0] = { pos.x - (ray), pos.y - ray };
			shape.ptls[1] = { pos.x + (ray), pos.y + ray };
		}
		return false;
	}
	else {
		int sumAngle = 180 * (shape.nPt - 2);
		float oneAngle = (float)sumAngle / shape.nPt;
		float angle = 180.0f - oneAngle;
		for (int i = 0; i < shape.nPt; ++i) {
			shape.ptls[i] = { pos.x + (LONG)(ray * cos(LKM::getRadian(i * angle))),
				pos.y - (LONG)(ray* sin(LKM::getRadian(i * angle))) };
		}
		return true;
	}
}

POINT LKM::rotatePoint(const POINT pos, const POINT dot, FLOAT rad)
{
	POINT len = { dot.x - pos.x, pos.y - dot.y };
	POINT temp;
	temp.x = pos.x + (LONG)(len.x * cos(rad) - len.y * sin(rad));
	temp.y = pos.y - (LONG)(len.x * sin(rad) + len.y * cos(rad));
	return temp;
}

FLOAT LKM::getRadian(int angle)
{
	const FLOAT PI = 3.1415926f;
	angle %= 360;
	return angle * PI / 180.0f;
}

int LKM::getUnit(double value)
{
	if (value > 0)
		return 1;
	else if (value < 0)
		return -1;
	else
		return 0;
}

FLOAT LKM::decreaseReal(FLOAT a, FLOAT b)
{
	if (a != 0) {
		return fabs(a) * (1 - (b / a));
	}
	return -b;
}

POINT LKM::exchange(POINTFLOAT t)
{
	return { (LONG)t.x, (LONG)t.y };
}

POINTFLOAT LKM::exchange(POINT t)
{
	return { (FLOAT)t.x, (FLOAT)t.y };
}

BOOL LKM::isCollide(Shape &, Shape &)
{
	return 0;
}

POINTFLOAT LKM::addForce(POINTFLOAT a, POINTFLOAT b)
{
	POINTFLOAT temp;
	temp.x = a.x + b.x;
	temp.y = a.y + b.y;
	return temp;
}

BOOL LKM::collideP2P(POINT lpt, POINT rpt, FLOAT interval)
{
	FLOAT length = LKM::getLength(LKM::exchange(lpt), LKM::exchange(rpt));
	return (interval >= length);
}

BOOL LKM::collideR2C(RECT* rect, POINT center, FLOAT len)
{
	rect->left -= len;
	rect->right += len;
	rect->top -= len;
	rect->bottom += len;
	if (PtInRect(rect, center)) {
		return true;
	}
	else {
		return false;
	}

}

BOOL LKM::collideS2L(const Shape * shape, POINT a, POINT b)
{
	for (int i = 0; i < shape->nPt; ++i) {
		int ii = (i + 1 == shape->nPt ? 0 : i + 1);
		if (crossLine(shape->ptls[i], shape->ptls[ii], a, b)) {
			return true;
		}
	}
	return false;
}

COLORREF LKM::BlendColor(COLORREF a, COLORREF b, FLOAT m)
{
	int rgb[3];
	rgb[0] = GetRValue(a) * m + GetRValue(b) * (1.0f - m);
	rgb[1] = GetGValue(a) * m + GetGValue(b) * (1.0f - m);
	rgb[2] = GetBValue(a) * m + GetBValue(b) * (1.0f - m);
	return RGB(rgb[0], rgb[1], rgb[2]);
}

FLOAT addFbyV(FLOAT b, FLOAT a) {
	int pm = 0;
	if (a != 0)
		pm = fabs(a) / (int)a;
	else
		return 0;
	double result = a + b;
	if (result / pm < 0)
		return 0;
	else
		return result;
}

FLOAT LKM::getLength(POINTFLOAT a, POINTFLOAT b) {
	POINT line = { a.x - b.x, a.y - b.y };
	double len = sqrt(line.x * line.x + line.y * line.y);
	return len;
}

FLOAT LKM::getLength(POINTFLOAT line)
{
	return sqrt(line.x * line.x + line.y * line.y);
}

RECT LKM::makeRect(POINT pos, LONG size)
{
	return { pos.x - size, pos.y - size, pos.x + size, pos.y + size };
}

void LKM::transRect2Shape(RECT * rect, Shape * shape)
{
	if (shape->nPt == 4) {
		shape->ptls[0] = { rect->left, rect->top };
		shape->ptls[1] = { rect->right, rect->top };
		shape->ptls[2] = { rect->right, rect->bottom };
		shape->ptls[3] = { rect->left, rect->bottom };
	}
}
