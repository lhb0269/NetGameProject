#pragma once
#include "LKM_Physics.h"
#include "Object.h"
//#include"TeamProject_2KnMn_1.h"
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


struct PlayerBullet {
	POINT pos;
	POINT lastPos;
	POINT start;
	POINTFLOAT velocity;

	PlayerBullet(POINT point, POINTFLOAT velo);
	~PlayerBullet();
	void translocate();

};

#define MAX_PLAYER_BULLET 10
class PlayerBulletManager {
	PlayerBullet* playerBullet[MAX_PLAYER_BULLET];
	int num;
public:
	PlayerBulletManager();
	~PlayerBulletManager();
	void add(POINT pos, POINTFLOAT velocity);
	void move(RECT * map);
	void destroy(int index);
	void draw(HDC hdc);
	int getNum();
	LKM::Shape* getBulletShape(int index);
};

enum PLAYER_STATE {
	USUAL, SLASH, BANG, DAMAGED, DISEXIST
};

class Player : public RealObject
{
	LONG coreRay;
	LONG orbitRay;
	char state;
	int numOfBullet;
	int numOfShell;
	float shellStack;
	bool bGameOver;
	FLOAT nowResis;
	POINTFLOAT tempForce;

	LKM::Shape fsword;

	LONG inputStack;
	clock_t isTouched;

	int bangMotion;

	void drawCore(HDC);
	void drawSword(HDC);
	void drawShell(HDC);
	void drawBullet(HDC);
	void slash(POINT, int);
	void stab(POINT, int charge);
	void setSwordShape();
	void drawSwordShadow(HDC hdc);
	void bang(POINT);
public:
	Player();
	~Player();

	Sword sword;
	PlayerBulletManager pbManager;

	void setPos(POINT);
	void start(POINT);
	void draw(HDC hdc);
	void Force(POINTFLOAT);
	void setResist(FLOAT);
	void input(BOOL[]);
	void move(RECT* sanctaury, RECT * wholeMap, RECT* block);
	void mouseInput(BOOL lbtn, BOOL rbtn, POINT mPos);
	void getSwordCollider(LKM::Shape*);
	const RECT getCore();
	LONG getOrbitRay();
	LONG getCoreRay();
	Sword& getSword();
	BOOL isDamaged();
	void beAttacked();
	bool gameovercheck();
};
