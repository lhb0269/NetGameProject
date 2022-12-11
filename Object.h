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
	POINT previous_pos;
	int	 state;

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

	void SetState(int n) { state = n; }
	int GetState() const { return state; };
	void SetPrePos(POINT pos) { previous_pos = pos; }
	POINT GetPrePos() const { return previous_pos; };
};