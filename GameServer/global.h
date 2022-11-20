#pragma once


#define MAX_MOB 100

#include <math.h>
#include <random>
#include <time.h>
#include <windows.h>
#include <vector>
#include "PACKET.h"
#include "EnemyManager.h"
#include "WaveManager.h"
#include "Tower.h"

using namespace std;

enum Enemy_Kind {
	HEADED, HEADLESS, TOWER, BOMBER, SLUG, SUN, ENEMY_KIND
};