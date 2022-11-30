#include "stdafx.h"
#include "EffectManager.h"
#include "Player.h"

//#pragma comment(lib, "msimg32.lib")

#define STAB_SPEED 50
#define WALK_SPEED 15

#define LIMIT_ANGLE 20

#define BULLET_SPEED 80

extern EffectManager effectMng;

void drawTestDot(HDC hdc, POINT pos)
{
	HPEN pen = CreatePen(PS_SOLID, 5, RGB(0xff, 0x0, 0x0));
	HPEN  oldpen = (HPEN)SelectObject(hdc, pen);
	Ellipse(hdc, pos.x - 1, pos.y - 1, pos.x + 1, pos.y + 1);
	SelectObject(hdc, oldpen);
	DeleteObject(pen);
}

Player::Player() : fsword(6)
{
	pos = { 0 ,0 };

	nowResis = 2;
	
	state = USUAL;

	coreRay = 20;
	orbitRay = coreRay * 2;
	
	sword.swordLength = (LONG)(orbitRay * 3.5f);
	sword.swordLength_s = sword.swordLength;
	sword.swordWidth = coreRay / 2;
	sword.nowAngle = 0;
	sword.swordRay = orbitRay;
	
	numOfShell = 4;
	shellStack = 0.0f;
	bGameOver = false;

	inputStack = 0;
	isTouched = clock();

	bangMotion = 0;

	id = -1;
}

Player::~Player()
{
}

void Player::setPos(POINT point)
{
	pos = point;
}

void Player::start(POINT startPoint)
{
	pos = startPoint;
}

const RECT Player::getCore()
{
	const LONG size = coreRay;
	RECT core = { pos.x - size, pos.y - size, pos.x + size, pos.y + size };
	return core;
}

LONG Player::getOrbitRay()
{
	return orbitRay;
}
LONG Player::getCoreRay()
{
	return coreRay;
}

Sword & Player::getSword()
{
	return sword;
}

BOOL Player::isDamaged()
{
	if (clock() - isTouched < 999)
		isdamaged = false;
	else
		isdamaged =  true;
	return isdamaged;
}

void Player::beAttacked()
{
	if (numOfShell == -1) {
		bGameOver = true;
	}
	numOfShell = numOfShell > 0 ? numOfShell - 1 : 0;
	effectMng.add(pos, Break);
	shellStack = 0;
	sword.swordLength = sword.swordLength_s;
	isTouched = clock();
}

void Player::setSwordShape()
{
	const POINT& swordPos = sword.swordPos;
	fsword.ptls[0] = { swordPos.x + sword.swordRay + sword.swordLength, swordPos.y }; // ���� �� ������������ �ð� �������� ��ǥ �ο�
	fsword.ptls[1] = { fsword.ptls[0].x - sword.swordWidth * 2, fsword.ptls[0].y + sword.swordWidth };
	fsword.ptls[2] = { swordPos.x + sword.swordRay, fsword.ptls[1].y };
	fsword.ptls[3] = { swordPos.x + sword.swordRay - sword.swordWidth * 2, swordPos.y };
	fsword.ptls[4] = { fsword.ptls[2].x, fsword.ptls[0].y - sword.swordWidth };
	fsword.ptls[5] = { fsword.ptls[1].x, fsword.ptls[4].y };

	for (int i = 0; i < fsword.nPt; ++i) {
		fsword.ptls[i] = LKM::rotatePoint(swordPos, fsword.ptls[i], sword.nowAngle);
	}

}


void Player::drawSwordShadow(HDC hdc)
{ 
	BOOL isSlash = (sword.isSlashNow()) & (fabs(sword.nowAngle - sword.lastAngle) > 0.01f);
	if (isSlash) {
		const FLOAT alpha = 0.6f;
		static 	COLORREF temp;
		temp = LKM::BlendColor(RGB(0xff, 0xff, 0xff), RGB(0xee, 0xe6, 0xcc), alpha);
		HBRUSH brush = CreateSolidBrush(temp);
		HBRUSH oldone = (HBRUSH)SelectObject(hdc, brush);
		HPEN pen = CreatePen(PS_SOLID, 10, temp);
		HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		LONG length = sword.getLength() + 10;
		POINT pos = fsword.ptls[3];
		RECT shadowBox = { pos.x - length, pos.y - length , pos.x + length , pos.y + length };
		POINTFLOAT start;
		POINTFLOAT end;
		if (sword.nowAngle - sword.lastAngle > 0) {
			start = { pos.x + length * cos(sword.lastAngle), pos.y - length * sin(sword.lastAngle) };
			end = { pos.x + length * cos(sword.nowAngle),pos.y - length * sin(sword.nowAngle) };
		}
		else if (sword.nowAngle - sword.lastAngle < 0) {
			end = { pos.x + length * cos(sword.lastAngle), pos.y - length * sin(sword.lastAngle) };
			start = { pos.x + length * cos(sword.nowAngle),pos.y - length * sin(sword.nowAngle) };
		}
		
		Pie(hdc, shadowBox.left, shadowBox.top, shadowBox.right, shadowBox.bottom, (LONG)start.x, (LONG)start.y, (LONG)end.x, (LONG)end.y);

		SelectObject(hdc, oldone);
		SelectObject(hdc, oldPen);
		DeleteObject(brush);
		DeleteObject(pen);
	}
}

void Player::bang(POINT sub)
{
	const LONG minLen = coreRay * 2;
	if (bangMotion == 0) {
		FLOAT len = LKM::getLength(LKM::exchange(sub));
		POINTFLOAT temp = { (FLOAT)sub.x / len * BULLET_SPEED, -(FLOAT)sub.y / len * BULLET_SPEED };
		POINTFLOAT swordEdge = { pos.x + sword.swordLength * cos(sword.nowAngle), pos.y - sword.swordLength * sin(sword.nowAngle) };
		Bangpos = LKM::exchange(swordEdge);
		velocity = temp;
		pbManager.add(Bangpos, temp);
		bangMotion = 10;
	}
	else {
		--bangMotion;
	}
}

void Player::drawCore(HDC hdc)
{
	//RECT rect = getCore();
	HBRUSH brush = CreateSolidBrush(RGB(0xe, 0xe, 0xe));

	switch (id) //id에 따른 플레이어 색깔 구분
	{
	case 0:	 // 검정
		brush = CreateSolidBrush(RGB(0, 0, 0));
		break;
	case 1: // 빨강
		brush = CreateSolidBrush(RGB(255, 0, 0));
		break;
	case 2: // 초록
		brush = CreateSolidBrush(RGB(0, 255, 0));
		break;
	case 3: // 파랑
		brush = CreateSolidBrush(RGB(0, 0, 255));
		break;
	}
	HBRUSH old = (HBRUSH)SelectObject(hdc, brush);
	HPEN  oldpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	LKM::Shape core(6);
	LKM::shapeHexagon(coreRay, pos, core);
	Polygon(hdc, core.ptls, core.nPt);
	//Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(hdc, old);
	SelectObject(hdc, oldpen);
	DeleteObject(brush);
}

void Player::drawSword(HDC hdc)
{
	HPEN  oldpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	drawSwordShadow(hdc);
	HBRUSH brush = CreateSolidBrush(RGB(0xff, 0xff, 0xff));
	HBRUSH old = (HBRUSH)SelectObject(hdc, brush);
	Polygon(hdc, fsword.ptls, fsword.nPt);
	SelectObject(hdc, oldpen);
	SelectObject(hdc, old);
	DeleteObject(brush);
}

void Player::drawShell(HDC hdc)
{
	RECT shell = { pos.x - orbitRay, pos.y - orbitRay, pos.x + orbitRay, pos.y + orbitRay };
	const int TermAngle = 15;
	HPEN pen = CreatePen(PS_SOLID, 5, RGB(0xff, 0xff, 0xff));
	HPEN  oldpen = (HPEN)SelectObject(hdc, pen);
	for (int i = 0; i < numOfShell; ++i) {
		POINT origin = { pos.x + orbitRay, pos.y };
		POINT start = LKM::rotatePoint(pos, origin, LKM::getRadian(TermAngle + 90 * i));
		POINT end = LKM::rotatePoint(pos, origin, LKM::getRadian(90 * (i + 1) - TermAngle));
		Arc(hdc, shell.left, shell.top, shell.right, shell.bottom, start.x, start.y, end.x, end.y);
	}
	if (!isdamaged) {
		orbitRay += 15;
		RECT shell = { pos.x - orbitRay, pos.y - orbitRay, pos.x + orbitRay, pos.y + orbitRay };
		orbitRay -= 15;
		HPEN pen = CreatePen(PS_SOLID, 5, RGB(0xff, 0xff, 0xff));
		HPEN  oldpen = (HPEN)SelectObject(hdc, pen);
		HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, shell.left, shell.top, shell.right, shell.bottom);
		SelectObject(hdc, oldpen);
		SelectObject(hdc, oldbrush);
		DeleteObject(pen);
	}
	SelectObject(hdc, oldpen);
	DeleteObject(pen);

	COLORREF temp = RGB(
		0xff * shellStack + 0xee * (1.0 - shellStack),
		(0xff * shellStack + 0xe6 * (1.0 - shellStack)),
		(0xff * shellStack + 0xcc * (1.0 - shellStack)));
	pen = CreatePen(PS_SOLID, 5, temp);
	oldpen = (HPEN)SelectObject(hdc, pen);
	if(numOfShell < 4) {
		if (shellStack >= 1.0f) {
			shellStack = 0.0f;
			numOfShell++;
		}
	}
	SelectObject(hdc, oldpen);
	DeleteObject(pen);
}

void Player::drawBullet(HDC hdc)
{
	pbManager.draw(hdc);
}

void Player::slash(POINT targetPos, int rotateType)
{
	sword.setRoate(rotateType * LKM::getRadian(5), 3);
}

void Player::stab(POINT targetPos, int charge)
{
	setForce({ NULL }, 0);
	velocity = { NULL };
	FLOAT targetAngle = sword.nowAngle;
	LONG power = 30 + charge;
	POINTFLOAT swordVector = { cos(targetAngle) * power, sin(targetAngle) * power * -1 };
	tempForce = LKM::addForce(tempForce, swordVector);
	inputStack = 10;
	//sword.nowAngle = (targetAngle);
	sword.stopRotate(false);
}


void Player::draw(HDC hdc)
{
	drawBullet(hdc);
	drawShell(hdc);
	drawSword(hdc);
	drawCore(hdc);
	//	POINT temp = { (LONG)(pos.x + cos(sword.nowAngle) * (sword.swordLength + coreRay)),
	//(LONG)(pos.y - sin(sword.nowAngle) * (sword.swordLength + coreRay)) };
	//	const LKM::Shape& swordf = getSwordCollider();
	//	for (int i = 0; i < swordf.nPt; ++i) {
	//		drawTestDot(hdc, swordf.ptls[i]);
	//	}
}

void Player::Force(POINTFLOAT force)
{
	tempForce = LKM::addForce(tempForce, force);
}

void Player::setResist(FLOAT re)
{
	if (re >= 0) {
		nowResis = re;
	}
}

void Player::input(BOOL buffer[])
{
	if (inputStack == 0) {
		const FLOAT move_force = 5;
		POINTFLOAT force = { NULL };
		if (buffer[0]) { // A
			force.x -= move_force;
		}
		if (buffer[1]) { // D
			force.x += move_force;
		}
		if (buffer[2]) { // S
			force.y += move_force;
		}
		if (buffer[3]) { // W
			force.y -= move_force;
		}
		Force(force);
	}
}

void Player::move(RECT* sanctaury, RECT* wholeMap, RECT* block)
{
	this->setForce(tempForce, isTouched ? nowResis / 2 : nowResis);
	tempForce = { NULL };
	applyForce();

	sword.rotate();
	if (block != nullptr) {
		translocate(WALK_SPEED, block);
	}
	else {
		translocate(WALK_SPEED);
	}
	inputStack = inputStack > 0 ? inputStack - 1 : 0;
	sword.swordPos = pos;
	if (PtInRect(sanctaury, pos)) {
		shellStack += numOfShell < 4 ? 0.008f : 0;
	}
	sword.swordPos.x -= (LONG)(cos(sword.nowAngle) * bangMotion + 0.5f);
	sword.swordPos.y += (LONG)(sin(sword.nowAngle) * bangMotion + 0.5f);
	setSwordShape();
	pbManager.move(wholeMap);
}

void Player::mouseInput(BOOL lbtn, BOOL rbtn, POINT mPos)
{
	static bool bSlash = false;
	POINT temp = { mPos.x - pos.x, pos.y - mPos.y };

	static int lastInput = 1;
	if (rbtn) {
		if (state <= BANG) {
			state = BANG;
			sword.nowAngle = (FLOAT)atan2(temp.y, temp.x);
			sword.stopRotate(true);
			bang(temp);
		}
	}else if (!rbtn && state == BANG) {
		state = USUAL;
		bangMotion = 0;
		sword.stopRotate(false);
		bSlash = false;
	}
	else if (lbtn) {
		if (!bSlash) {
			bSlash = true;
			state = SLASH;
			slash(temp, lastInput);
		}
	}
	else if(!lbtn && state > USUAL) {
		lastInput *= -1;
		bSlash = false;
		state = USUAL;
		sword.stopRotate(false);
	}
}

void Player::getSwordCollider(LKM::Shape* hitBox)
{
	if (hitBox->nPt < fsword.nPt)
		return;
	LKM::Shape& swordc = *hitBox;
	for (int i = 0; i < swordc.nPt; ++i) {
		swordc.ptls[i] = fsword.ptls[i];
	}
	if (sword.isSlashNow()) {
		if (sword.nowAngle - sword.lastAngle > 0) {
			swordc.ptls[0].x += (LONG)(cos(sword.nowAngle) * (10.0f));
			swordc.ptls[0].y -= (LONG)(sin(sword.nowAngle) * (10.0f));
			swordc.ptls[1] = LKM::rotatePoint(pos, swordc.ptls[1], sword.lastAngle - sword.nowAngle);
			swordc.ptls[2] = LKM::rotatePoint(pos, swordc.ptls[2], sword.lastAngle - sword.nowAngle);
			swordc.ptls[3] = pos;
		}
		else {
			swordc.ptls[0].x += (LONG)(cos(sword.nowAngle) * (10.0f));
			swordc.ptls[0].y -= (LONG)(sin(sword.nowAngle) * (10.0f));
			swordc.ptls[5] = LKM::rotatePoint(pos, swordc.ptls[5], sword.lastAngle - sword.nowAngle);
			swordc.ptls[4] = LKM::rotatePoint(pos, swordc.ptls[4], sword.lastAngle - sword.nowAngle);
			swordc.ptls[3] = pos;
		}
	}
}

Sword::Sword()
{
	isSlash = false;
	inertia = 1;
}

Sword::~Sword()
{
}

void Sword::setRoate(FLOAT moment, LONG inert)
{
	if (!isSlash) {
		swordRay *= 1.3f;
	}
	isSlash = true;
	rotMoment = moment;
	inertia = inert;
}

void Sword::stopRotate(bool stopRightNow)
{
	if (isSlash) {
		swordRay /= 1.3f;
	}
	isSlash = false;
	rotMoment = 0;
	numOfRotate = 0;
	swordLength = swordLength_s;
	if (stopRightNow) {
		rotVelocity = 0;
	}
}

FLOAT Sword::getRotVelocity()
{
	return rotVelocity / (inertia * 2);
}

void Sword::rotate() {
	rotVelocity += (rotMoment - getRotVelocity());
	rotVelocity = fabs(rotVelocity) < fabs(LKM::getRadian(LIMIT_ANGLE)) ? rotVelocity : LKM::getUnit(rotVelocity) * fabs(LKM::getRadian(LIMIT_ANGLE));
	lastAngle = nowAngle;
	nowAngle += rotVelocity;

	if (isSlash) {
		numOfRotate++;
		swordLength > 0 ? --swordLength : 0;
	}

}

void Sword::addLength(LONG temp)
{
	swordLength += temp;
	if (swordLength < swordLength_s) {
		swordLength = swordLength_s;
	}
}

LONG Sword::getLength()
{
	return swordLength;
}

bool Sword::isSlashNow()
{
	return isSlash;
}


// Player's Bullet Class

PlayerBulletManager::PlayerBulletManager()
{
	num = 0;
}

PlayerBulletManager::~PlayerBulletManager()
{
	//for (int i = 0; i < num; ++i)
	//	delete playerBullet[i];
}

void PlayerBulletManager::add(POINT pos, POINTFLOAT velocity)
{
	if(num < MAX_PLAYER_BULLET)
		playerBullet[num++] = new PlayerBullet(pos, velocity);
}

void PlayerBulletManager::move(RECT * map)
{
	for (int i = 0; i < num; ++i) {
		playerBullet[i]->translocate();
		if (!PtInRect(map, playerBullet[i]->pos)) {
			destroy(i);
			i--;
		}
	}
}

void PlayerBulletManager::destroy(int index)
{
	delete playerBullet[index];
	playerBullet[index] = playerBullet[--num];
}

void PlayerBulletManager::draw(HDC hdc)
{
	HPEN pen = CreatePen(PS_SOLID, 10, LKM::BlendColor(RGB(0xff, 0xff, 0xff), RGB(0xee, 0xe6, 0xcc), 0.8f));
	HPEN oldp = (HPEN)SelectObject(hdc, pen);
	for (int i = 0; i < num; ++i) {
		MoveToEx(hdc, playerBullet[i]->start.x, playerBullet[i]->start.y, NULL);
		LineTo(hdc, playerBullet[i]->pos.x, playerBullet[i]->pos.y);

		LKM::Shape* temp = getBulletShape(i);

		//for (int j = 0; j < temp->nPt; ++j) {
		//	drawTestDot(hdc, temp->ptls[j]);
		//}
	}
	SelectObject(hdc, oldp);
	DeleteObject(pen);
}

int PlayerBulletManager::getNum()
{
	return num;
}

LKM::Shape * PlayerBulletManager::getBulletShape(int index)
{
	static LKM::Shape form(2);
	form.ptls[0] = playerBullet[index]->lastPos;
	form.ptls[1] = playerBullet[index]->pos;
	return &form;
}



// 


PlayerBullet::PlayerBullet(POINT point, POINTFLOAT velo)
{
	pos = point;
	start = point;
	lastPos = point;
	velocity = velo;
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::translocate()
{
	lastPos = pos;

	pos.x += velocity.x;
	pos.y += velocity.y;
}

bool Player::gameovercheck()
{
	return bGameOver;
}