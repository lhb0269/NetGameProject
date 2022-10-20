#pragma once
#include "LKM_Physics.h"

class ScoreBoard
{
	LONG score;
	
public:
	ScoreBoard();
	~ScoreBoard();
	void addScore(LONG addr);
	void drawScore(HDC, POINT camera, POINT cameraSize, COLORREF thema);
	void resetScore();
};

