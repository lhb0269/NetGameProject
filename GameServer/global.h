#pragma once


#define MAX_MOB 100


#include"PACKET.h"
#include<math.h>
#include<random>
#include<time.h>
#include <windows.h>
#include"EnemyManager.h"
#include"WaveManager.h"
#include "Tower.h"


enum Enemy_Kind {
	HEADED, HEADLESS, TOWER, BOMBER, SLUG, SUN, ENEMY_KIND
};