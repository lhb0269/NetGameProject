#include "stdafx.h"
#include "ScoreBoard.h"


ScoreBoard::ScoreBoard()
{
}


ScoreBoard::~ScoreBoard()
{
}

void ScoreBoard::addScore(LONG addr)
{
	score += addr;
}

void ScoreBoard::drawScore(HDC hdc, POINT camera, POINT cameraSize, COLORREF thema)
{
	RECT board = { camera.x, camera.y + cameraSize.y / 3, camera.x + cameraSize.x, camera.y + cameraSize.y / 3 * 2 }; 
	HBRUSH hbrush = CreateSolidBrush(LKM::BlendColor(thema, RGB(0xff, 0xff, 0xff), 0.5));
	FillRect(hdc, &board, hbrush);
	
}
