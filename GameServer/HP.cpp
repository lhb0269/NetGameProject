#include "HP.h"

HP::HP(Object* owner, int Width, int height) : Owner(owner), Width(Width), Height(height)
{
	MaxHp = 100;
	NowHP = 100;
	AnimationHp = Width * 2;
	Pos.x = 0;
	Pos.y = 25;
}

HP::~HP()
{

}

void HP::update()
{
	OwnerPos = Owner->getPos();
}

void HP::SetHP(int x)
{
	NowHP = x;
	AnimationHp = ((Width * 2) * NowHP) / MaxHp;
}

void HP::Add_damage(int d)
{
	if (NowHP > 0)
	{
		NowHP -= d;
		AnimationHp = ((Width * 2) * NowHP) / MaxHp;
	}
}

void HP::draw(HDC hdc)
{
	//Hp BG
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(BGColor.R, BGColor.G, BGColor.B));
	HBRUSH h_brush = (HBRUSH)CreateSolidBrush(RGB(BGColor.R, BGColor.G, BGColor.B));
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, h_brush);

	Rectangle(hdc, OwnerPos.x - Width + Pos.x, OwnerPos.y + Pos.y,
		OwnerPos.x + Width + Pos.x, OwnerPos.y + Pos.y + Height);

	//HP bar
	pen = CreatePen(PS_SOLID, 1, RGB(HPColor.R, HPColor.G, HPColor.B));
	h_brush = (HBRUSH)CreateSolidBrush(RGB(HPColor.R, HPColor.G, HPColor.B));
	oldpen = (HPEN)SelectObject(hdc, pen);
	old_brush = (HBRUSH)SelectObject(hdc, h_brush);

	Rectangle(hdc, OwnerPos.x - Width + Pos.x, OwnerPos.y + Pos.y,
		OwnerPos.x - Width + AnimationHp + Pos.x, OwnerPos.y + Pos.y + Height);

	SelectObject(hdc, old_brush);
	DeleteObject(h_brush);
	SelectObject(hdc, oldpen);
	DeleteObject(pen);
}

void HP::SetBGColor(int r, int g, int b)
{
	BGColor.R = r;
	BGColor.G = g;
	BGColor.B = b;
}

void HP::SetHPColor(int r, int g, int b)
{
	HPColor.R = r;
	HPColor.G = g;
	HPColor.B = b;
}
