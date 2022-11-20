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

class Sword {
	FLOAT rotMoment;
	FLOAT rotVelocity;
	LONG inertia;
	int numOfRotate;
	int pnm;
	bool isSlash;
	POINT swordPos;
	LONG swordLength;
	LONG swordLength_s;
	LONG swordWidth;
	FLOAT nowAngle;
	FLOAT lastAngle;
	LONG swordRay;
};