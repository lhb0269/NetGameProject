#pragma once
#include "LKM_Physics.h"
#include "Block.h"

#define MAX_BLOCK 30

class MapManager
{
	const POINT wholeSize;
	POINT center;
	POINT camera;
	POINT cameraSize;
	LONG radius;
	COLORREF mainthema;
	Block* blockList[MAX_BLOCK];
	int blockNum;

	void drawFrontier(HDC);
	void drawBackground(HDC);
	void drawBlock(HDC);
public:
	MapManager(POINT);
	~MapManager();
	RECT getWholeMapRect();
	RECT getMapRect();
	POINT getWholeMapSize();
	POINT getCameraPoint();
	POINT getCameraSize();
	RECT getCameraRect();

	COLORREF getThemaColor();
	
	BOOL addBlock(POINT point, int type);
	BOOL movalbeTest(LKM::Shape* pp, POINT velocity, RECT& some);
	BOOL IntersectBlock(const LKM::Shape* shape);
	BOOL IntersectBlock(RECT* rect);
	BOOL destroyBlock(int index);
	BOOL beTouchedBlock(const LKM::Shape* shape);

	void ScreenToGame(POINT& sPos);
	void update(HWND hWnd, POINT player);
	void draw(HDC);
};

