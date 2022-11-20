#pragma once
class Sword {
	FLOAT rotMoment;
	FLOAT rotVelocity;
	LONG inertia;
	int numOfRotate;
	int pnm;
	bool isSlash;

public:
	POINT swordPos;
	LONG swordLength;
	LONG swordLength_s;
	LONG swordWidth;
	FLOAT nowAngle;
	FLOAT lastAngle;
	LONG swordRay;
	Sword();
	~Sword();
	void setRoate(FLOAT unitAngle, LONG time);
	void stopRotate(bool stopSign);
	FLOAT getRotVelocity();
	void rotate();
	void addLength(LONG);
	LONG getLength();
	bool isSlashNow();
};