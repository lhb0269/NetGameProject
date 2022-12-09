#pragma once
#include "stdafx.h"
#include "Object.h"
struct RGB {
	int R;
	int G;
	int B;
};
class HP {
	int MaxHp;
	int NowHP;
	int AnimationHp;
	int Width;
	int Height;
	POINT OwnerPos;
	POINT Pos;
	Object* Owner;
	RGB BGColor{0,0,0};
	RGB HPColor{255,0,0};
	
public:
	HP(Object* owner,int width,int height);
	~HP();

	void SetHP(int x) { NowHP = x; }
	int	GetHP() { return NowHP; }

	void SetBGColor(int r, int g, int b);
	void SetHPColor(int r, int g, int b);
	void SetHPWidth(int s) { Width = s; }
	void SetHPHeight(int h) { Height = h; }
	void SetPosition(LONG x, LONG y) { Pos.x = x; Pos.y = y; }

  	void Add_damage(int d);
	void update();
	void draw(HDC);
};