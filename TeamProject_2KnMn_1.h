#pragma once

#include "resource.h"
#include "Player.h"
#include "Monster.h"
#include"EnemyManager.h"
#include "MapManager.h"
#include "BulletManager.h"
#include "WaveManager.h"

void update(HWND hWnd, BOOL buffer[]);
void draw(HDC);
#ifndef bool bStart
bool bStart = false;
#endif
