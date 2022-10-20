#pragma once
#include "Object.h"

class Block :
	public Object
{
	static LONG size;

	bool threarening;
	bool breakable;

public:
	Block(POINT, int);
	~Block();
	BOOL beTouched(const LKM::Shape*);
	void draw(HDC hdc);
	RECT getRect();
	BOOL isBreakable();
};

