#pragma once

// 11.19 - ����
#define MAX_PLAYER 4

#define MAX_MOB 100
#include <math.h>
#include <random>
#include <time.h>
#include <vector>
#include "gWinSock.h"
#include "PACKET.h"
#include "EnemyManager.h"
#include "WaveManager.h"
#include "PlayerInfoManager.h"
#include"UIManager.h"
#include "Tower.h"
#include "debug_test.h"


using namespace std;

enum Enemy_Kind {
	HEADED, HEADLESS, TOWER, BOMBER, SLUG, SUN, ENEMY_KIND
};
