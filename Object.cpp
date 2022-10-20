#include "stdafx.h"
#include "Object.h"

static int Cnt;


POINTFLOAT RealObject::friction() {
	FLOAT len = LKM::getLength(velocity);
	POINTFLOAT frc = { (velocity.x / len * resistance * -1), velocity.y / len * resistance * -1 };
	if (len > 0 ) {
		return frc;
	}
	else {
		return { 0, 0 };
	}
}

void RealObject::translocate(FLOAT limitSpeed)
{
	FLOAT len = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	if (limitSpeed > 0 && len > limitSpeed) {
		velocity.x *= limitSpeed / len;
		velocity.y *= limitSpeed / len;
	}
	pos.x += velocity.x + LONG((double)LKM::getUnit(velocity.x) * 0.5);
	pos.y += velocity.y + LONG((double)LKM::getUnit(velocity.y) * 0.5);
}

void RealObject::translocate(FLOAT limitSpeed, RECT* rect)
{
	float vx, vy;
	POINT temp = { (LONG)(velocity.x), 0 };
	POINT pp = { pos.x + (LONG)temp.x,  pos.y + (LONG)temp.y };;
	while (LKM::collideR2C(rect, pp, 1)) {
		pp = { pos.x + (LONG)temp.x,  pos.y + (LONG)temp.y };
		temp.x -= LKM::getUnit(temp.x);
		if (temp.x == 0) {
			break;
		}
	}
	vx = temp.x;
	temp = { 0, (LONG)(velocity.y) };
	pp = { pos.x + (LONG)temp.x,  pos.y + (LONG)temp.y };
	while (LKM::collideR2C(rect, pp, 10)) {
		pp = { pos.x + (LONG)temp.x,  pos.y + (LONG)temp.y };
		temp.y -= LKM::getUnit(temp.y);
		if (temp.y == 0)
			break;
	}
	vy = temp.y;
	velocity = { vx, vy };
	translocate(limitSpeed);
}

void RealObject::applyForce()
{
	if (weight > 0) {
		POINTFLOAT firc = friction();
		POINT vUnit = { LKM::getUnit(velocity.x), LKM::getUnit(velocity.y) };
		POINTFLOAT temp = LKM::addForce(force, firc);
		velocity.x += temp.x / weight;
		velocity.y += temp.y / weight;
		if (LKM::getUnit(velocity.x) * vUnit.x < 0) {
			velocity.x = 0;
		}
		if (LKM::getUnit(velocity.y) * vUnit.y < 0) {
			velocity.y = 0;
		}

		force = {NULL};
	}
}

BOOL RealObject::setWeight(const FLOAT chanWeight)
{
	if (chanWeight <= 0) {
		return false;
	}
	else {
		weight = chanWeight;
		return true;
	}
}

void RealObject::setForce(POINTFLOAT iforce, FLOAT resis)
{
	force = iforce;
	if (resis >= 0) {
		resistance = resis;
	}
}

RealObject::RealObject()
{
	pos = { NULL };
	velocity = { NULL };
	angle = NULL;
	force = { NULL };
	weight = 1;
	resistance = 0;
}


RealObject::~RealObject()
{
}

Object::Object()
{
	pos = { NULL };
	angle = 0;
}

Object::~Object()
{
}

const POINT Object::getPos()
{
	return pos;
}

const POINTFLOAT RealObject::getForce()
{
	return force;
}

const POINTFLOAT RealObject::getVelocity()
{
	return velocity;
}

const FLOAT RealObject::getWeight()
{
	return weight;
}

const int Object::getAngle()
{
	return angle;
}
