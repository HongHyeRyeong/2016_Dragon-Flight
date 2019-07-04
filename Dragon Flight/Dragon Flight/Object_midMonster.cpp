#include "Object_midMonster.h"

void MidMonster::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP4));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP11));
	hpEmpty = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP32));
	hpMask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP19));
	hpFull = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP20));
	GetObject(resource, sizeof(BITMAP), &bmp);
	this->memDC = memDC;
	this->backDC = backDC;
	isDead = false;
	w = bmp.bmWidth;
	h = bmp.bmHeight;
	x = 0;
	y = 0 - h;
	hp = 100;
	attack = 100;
	frame = 0;
	frame_t = 0.f;
	speed = 80.f;
	isShop = false;
	collisionRC = { (LONG)(x), (LONG)y, (LONG)(x + 100), (LONG)(y + h) };
}

void MidMonster::update(double time)
{
	frame_t += time;
	if (frame_t > 0.2f)
	{
		frame_t = 0;
		++frame;
		frame %= 4;
	}

	if (y + h < SCREEN_HEIGHT)
		y += speed * time;
	else
		isDead = true;

	//if (hp < 0)
	//	isDead = true;
	collisionRC = { (LONG)(x), (LONG)y, (LONG)(x + 100), (LONG)(y + 100) };
}

void MidMonster::draw()
{
	SetStretchBltMode(backDC, COLORONCOLOR);

	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, w / 4, h, memDC, w / 4 * frame, 0, SRCAND);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, w / 4, h, memDC, w / 4 * frame, 0, SRCPAINT);

	//	HP¹Ù
	SelectObject(memDC, hpMask);
	BitBlt(backDC, x + 15, y + 80, 70, 10, memDC, 0, 0, SRCAND);
	SelectObject(memDC, hpEmpty);
	BitBlt(backDC, x + 15, y + 80, 70, 10, memDC, 0, 0, SRCPAINT);
	SelectObject(memDC, hpFull);
	switch (hp)
	{
	case 100:
		StretchBlt(backDC, x + 15, y + 80, 70, 10, memDC, 0, 0, 70, 10, SRCCOPY);
		break;
	case 75:
		StretchBlt(backDC, x + 15, y + 80, 50, 10, memDC, 0, 0, 70, 10, SRCCOPY);
		break;
	case 50:
		StretchBlt(backDC, x + 15, y + 80, 30, 10, memDC, 0, 0, 70, 10, SRCCOPY);
		break;
	case 25:
		StretchBlt(backDC, x + 15, y + 80, 10, 10, memDC, 0, 0, 70, 10, SRCCOPY);
		break;
	case 0:
		StretchBlt(backDC, x + 15, y + 80, 0, 10, memDC, 0, 0, 70, 10, SRCCOPY);
		break;
	}

}
