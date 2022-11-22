#pragma once
#include "LKM_Physics.h"

class Object
{
protected:
	POINT pos;
	FLOAT angle;
public:
	Object();
	~Object();
	const POINT getPos();
	const int getAngle();
};

class RealObject : public Object
{
	POINTFLOAT force;
	FLOAT resistance;
	FLOAT weight;
	POINTFLOAT friction();
protected:
	POINTFLOAT velocity;

	void translocate(FLOAT);
	void translocate(FLOAT, RECT*);
	void applyForce();
	BOOL setWeight(const FLOAT);
	void setForce(POINTFLOAT, FLOAT resistance = 0);
public:
	RealObject();
	~RealObject();
	const POINTFLOAT getForce();
	const POINTFLOAT getVelocity();
	const FLOAT getWeight();
};

//class Sword {
//	FLOAT rotMoment;
//	FLOAT rotVelocity;
//	LONG inertia;
//	int numOfRotate;
//	int pnm;
//	bool isSlash;
//	POINT swordPos;
//	LONG swordLength;
//	LONG swordLength_s;
//	LONG swordWidth;
//	FLOAT nowAngle;
//	FLOAT lastAngle;
//	LONG swordRay;
//};

struct Sword {
	FLOAT rotMoment = 0.f;
	FLOAT rotVelocity = 0.f;
	LONG inertia = 0;
	int numOfRotate = 0;
	int pnm = 0;
	bool isSlash = 0;
	POINT swordPos = { 0, 0 };
	LONG swordLength = 0;
	LONG swordLength_s = 0;
	LONG swordWidth = 0;
	FLOAT nowAngle = 0.f;
	FLOAT lastAngle = 0.f;
	LONG swordRay = 0;
};