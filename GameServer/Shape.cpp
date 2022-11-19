#include "global.h"
#include "LKM_Physics.h"

using namespace LKM;

Shape::Shape(const int nPoint):nPt(nPoint)
{
	ptls = new POINT[nPt];
	for (int i = 0; i < nPt; ++i) {
		ptls[i] = { NULL };
	}
}


Shape::~Shape()
{
	if (nPt >= 1) {
		delete[] ptls;
	}
}

void LKM::Shape::translocate(POINT interval)
{
	for (int i = 0; i < nPt; ++i) {
		ptls[i].x += interval.x;
		ptls[i].y += interval.y;
	}
}

BOOL LKM::Shape::Colide2Point(POINT point)
{
	int counter = 0;
	for (int i = 0; i < nPt; ++i) {
		int j = i + 1 < nPt ? i + 1 : (i + 1) % nPt;
		if ((ptls[i].y > point.y) != (point.y < ptls[j].y)) {
			FLOAT temp = (ptls[j].x - ptls[i].x) * (point.y - ptls[i].y) / (ptls[j].y - ptls[i].y) + ptls[i].x;
			if (point.x < temp) {
				counter++;
			}
		}
	}
	if (counter % 2 > 0)
		return true;
	else
		return false;
}

int ccw(POINT a, POINT b, POINT c) {
	int result = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
	return result > 0 ? 1 : (result < 0 ? -1 : 0);
}

void swap(POINT& a, POINT& b) {
	POINT temp = a;
	b = a;
	a = temp;
}

RECT LKM::makeRect(POINT lt, POINT rb) {
	if (lt.x > rb.x) {
		LONG temp = lt.x;
		lt.x = rb.x;
		rb.x = temp;
	}
	if (lt.y > rb.y) {
		LONG temp = lt.y;
		lt.y = rb.y;
		rb.y = temp;
	}
	return { lt.x, lt.y, rb.x, rb.y };
}

BOOL LKM::crossLine(POINT a, POINT  b, POINT c, POINT d) {
	int ab = ccw(a, b, c)*ccw(a, b, d);
	int cd = ccw(c, d, a)*ccw(c, d, b);
	if (ab == 0 && cd == 0) {
		RECT abrect = makeRect(a, b);
		RECT cdrect = makeRect(d, c);
		RECT temp = { NULL };
		return IntersectRect(&temp, &abrect, &cdrect);
	}
	return ab <= 0 && cd <= 0;
}

BOOL LKM::Shape::Collide2Shape(const Shape * other)
{
	for (int i = 0; i < this->nPt; ++i) {
		for (int j = 0; j < other->nPt; ++j) {
			int ii = (i + 1 == nPt ? 0 : i + 1);
			int jj = (j + 1 == other->nPt ? 0 : j + 1);
			if (crossLine(ptls[i], ptls[ii], other->ptls[j], other->ptls[jj])) {
				return true;
			}
		}
	}
	return false;
}

BOOL LKM::Shape::Colide2Line(POINT s, POINT e)
{
	for (int i = 0; i < this->nPt; ++i) {
		int ii = (i + 1 == nPt ? 0 : i + 1);
		if (crossLine(ptls[i], ptls[ii], s, e)) {
			return true;
		}
	}
	return false;
}
