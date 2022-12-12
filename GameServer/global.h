#pragma once

// 11.19 - ����
#define MAX_PLAYER 4

#define MAX_MOB 100
#define SWORD_DAMAGE 20
#define PLAYER_BULLET_DAMAGE 20
#define ENEMY_BULLET_DAMAGE 20

#include <math.h>
#include <random>
#include <time.h>
#include <vector>
#include <array>
#include <iostream>
#include "gWinSock.h"
#include "PACKET.h"
#include "EnemyManager.h"
#include "WaveManager.h"
#include "BulletManager.h"
#include "PlayerInfoManager.h"
#include"UIManager.h"
#include "Tower.h"
#include "debug_test.h"

using namespace std;

enum Enemy_Kind {
	HEADED, HEADLESS, TOWER, BOMBER, SLUG, SUN, ENEMY_KIND
};