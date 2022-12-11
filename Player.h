#pragma once
#include "LKM_Physics.h"
#include "Object.h"
#include "HP.h"

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

#define MAX_PLAYER_BULLET 100
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

#define MAX_PLAYER 4
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
	POINT Bangpos;
	POINTFLOAT velocity;

	int id;
	BOOL isdamaged;

	void drawCore(HDC);
	void drawSword(HDC);
	void drawShell(HDC);
	void drawBullet(HDC);
	void slash(POINT, int);
	void stab(POINT, int charge);
	void drawSwordShadow(HDC hdc);
	void bang(POINT);
public:
	Player();
	~Player();

	HP	hp{this,15,7};
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
	void beAttacked(int damage);
	bool gameovercheck();
	void setSwordShape();

	int GetId() { return id; }
	void SetId(int x) { id = x; }
	void SetSword(Sword& s) { sword = s; }

	void SetNumOfShell(int x) { numOfShell = x; }
	void SetisTouched(clock_t& t) { isTouched = t; }
	void SetorbitRay(LONG x) { orbitRay = x; }
	void SetshellStack(float s) { shellStack = s; }
	void Setisdamaged(BOOL x) { isdamaged = x; }
	void SetbangMotion(int b) { bangMotion = b; }
	void SetBangpos(POINT p) { Bangpos = p; }
	void SetVelocity(POINTFLOAT f) { velocity = f; }

	int GetNumOfShell() { return numOfShell; }
	clock_t GetisTouched() { return isTouched; }
	LONG GetorbitRay() { return orbitRay; }
	float GetshellStack() { return shellStack; }
	BOOL Getisdamaged() { return isdamaged; }
	int GetbangMotion() { return bangMotion; }
	POINT GetBangpos() { return Bangpos; }
	POINTFLOAT GetVelocity() { return velocity; }

};
