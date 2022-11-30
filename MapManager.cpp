#include "stdafx.h"
#include "MapManager.h"


MapManager::MapManager(POINT maps):wholeSize(maps)
{
	radius = MAP_RAY;
	center = { wholeSize.x / 2, wholeSize.y / 2 };
	mainthema = RGB(0xee, 0xe6, 0xcc);
	cameraSize = { 1280, 800 };
	camera = { wholeSize.x - cameraSize.x / 2, wholeSize.y - cameraSize.y / 2 };
}

MapManager::~MapManager()
{
	for (int i = 0; i < blockNum; ++i) {
		delete blockList[i];
	}
}

RECT MapManager::getMapRect()
{
	RECT temp = { center.x - radius, center.y - radius, center.x + radius, center.y + radius };
	return temp;
}

POINT MapManager::getWholeMapSize()
{
	return wholeSize;
}

RECT MapManager::getWholeMapRect()
{
	return { 0, 0, wholeSize.x, wholeSize.y };
}

POINT MapManager::getCameraPoint() {
	return camera;
}

POINT MapManager::getCameraSize() {
	return cameraSize;
}

RECT MapManager::getCameraRect()
{
	return { camera.x, camera.y, camera.x + cameraSize.x, camera.y + cameraSize.y };
}

COLORREF MapManager::getThemaColor()
{
	return mainthema;
}

BOOL MapManager::addBlock(POINT point, int type)
{
	if (blockNum >= MAX_BLOCK)
		return false;
	RECT newRect = LKM::makeRect(point, BLOCK_SIZE);
	if (IntersectBlock(&newRect)) {
		return false;
	}
	blockList[blockNum] = new Block(point, type);
	blockNum++;
	return true;
}

BOOL MapManager::movalbeTest(LKM::Shape* pp, POINT velocity, RECT& some)
{
	pp->translocate(velocity);
	LKM::Shape block(4);
	for (int i = 0; i < blockNum; ++i) {
		RECT rect = blockList[i]->getRect();
		LKM::transRect2Shape(&rect, &block);
		if (block.Collide2Shape(pp)) {
			some = rect;
			return false;
		}
	}
	return true;
}

BOOL MapManager::IntersectBlock(const LKM::Shape * shape)
{
	LKM::Shape block(4);
	for (int i = 0; i < blockNum; ++i) {
		RECT rect = blockList[i]->getRect();
		LKM::transRect2Shape(&rect, &block);
		if (block.Collide2Shape(shape)) {
			return true;
		}
	}
	return false;
}

BOOL MapManager::IntersectBlock(RECT * rect)
{
	for (int i = 0; i < blockNum; ++i) {
		RECT block = blockList[i]->getRect();
		RECT temp;
		if (IntersectRect(&temp, &block, rect)) {
			return true;
		}
	}
	return false;
}

BOOL MapManager::destroyBlock(int index)
{
	if (index >= blockNum) {
		return false;
	}
	delete blockList[index];
	blockList[index] = blockList[--blockNum];
}

BOOL MapManager::beTouchedBlock(const LKM::Shape * hitBox)
{
	for (int i = 0; i < blockNum; ++i) {
		if (blockList[i]->beTouched(hitBox)) {
			if(blockList[i]->isBreakable())
				destroyBlock(i);
			return true;
		}
	}
	return false;
}

void MapManager::ScreenToGame(POINT & sPos)
{
	sPos.x += camera.x;
	sPos.y += camera.y;
}

void MapManager::update(HWND hWnd, POINT player)
{
	RECT win;
	GetClientRect(hWnd, &win);
	POINT mPos;
	GetCursorPos(&mPos);
	POINT screen = { GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN) };
	ScreenToGame(mPos);
	mPos = { mPos.x - player.x, mPos.y - player.y };
	FLOAT len = LKM::getLength(LKM::exchange(mPos));
	const FLOAT maxLen = 100;
	if (len > maxLen) {
		mPos.x *= (LONG)(maxLen / len);
		mPos.y *= (LONG)(maxLen / len);
	}

	camera = { player.x  - cameraSize.x/2, player.y - cameraSize.y/2 };
	camera = { camera.x < 0 ? 0 : camera.x, camera.y < 0 ? 0 : camera.y };
	camera = { camera.x + win.right > wholeSize.x ? wholeSize.x - win.right : camera.x,
				camera.y + win.bottom > wholeSize.y ? wholeSize.y - win.bottom : camera.y };
}

void MapManager::draw(HDC hdc)
{
	drawBackground(hdc);
	drawFrontier(hdc);
	drawBlock(hdc);
}

void MapManager::drawFrontier(HDC memDC)
{
	HPEN pen = CreatePen(PS_SOLID, 5, RGB(0xff, 0xff, 0xff));
	HPEN oldp = (HPEN)SelectObject(memDC, pen);
	HBRUSH oldb = (HBRUSH)SelectObject(memDC, GetStockObject(NULL_BRUSH));
	RECT mapSize = getMapRect();
	Ellipse(memDC, mapSize.left, mapSize.top, mapSize.right, mapSize.bottom);
	SelectObject(memDC, oldb);
	SelectObject(memDC, oldp);
	DeleteObject(oldp);
	DeleteObject(oldb);
	DeleteObject(pen);
}

void MapManager::drawBackground(HDC hdc) {
	HBRUSH tbrush = CreateSolidBrush(mainthema);
	RECT map = getWholeMapRect();
	FillRect(hdc, &map, tbrush);
	DeleteObject(tbrush);
}

void MapManager::drawBlock(HDC hdc) {

	for (int i = 0; i < blockNum; ++i) {
		blockList[i]->draw(hdc);
	}
}