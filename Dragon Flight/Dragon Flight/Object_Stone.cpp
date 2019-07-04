#include "Object_Stone.h"

void Stone::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP45));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP46));
	this->memDC = memDC;
	this->backDC = backDC;
	GetObject(resource, sizeof(BITMAP), &bmp);
	x = 0;
	y = 0;
	w = bmp.bmWidth;
	h = bmp.bmHeight;
	speed = 200;
	isDead = false;
	isActive = false;
	collisionRC = { (LONG)x, (LONG)y, (LONG)(x + w), (LONG)(y + h) };
}

void Stone::update(double time)
{
	if (y + h < SCREEN_HEIGHT)
	{
		isDead = false;
		y += speed * time;
	}
	else
	{
		isDead = true;
	}
	collisionRC = { (LONG)x, (LONG)y, (LONG)(x + w), (LONG)(y + h) };
}

void Stone::draw()
{
	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, w, h, memDC, 0, 0, SRCPAINT);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, w, h, memDC, 0, 0, SRCAND);
}
