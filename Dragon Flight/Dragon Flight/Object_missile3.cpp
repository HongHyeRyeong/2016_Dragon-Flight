#include "Object_missile3.h"

void Missile3::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP39));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP40));
	this->memDC = memDC;
	this->backDC = backDC;
	GetObject(resource, sizeof(BITMAP), &bmp);
	x = 0;
	y = 0;
	w = x + 30;
	h = bmp.bmHeight;
	speed = 800;
	frame_t = 0;
	isDead = false;
	isActive = false;
	collisionRC = { (LONG)x, (LONG)y, (LONG)(x + 30), (LONG)(y + h) };
	attack = 75;
}

void Missile3::update(double time)
{
	if (y > 0)
	{
		isDead = false;
		y -= speed * time;
	}
	else
	{
		isDead = true;
	}
	collisionRC = { (LONG)x, (LONG)y, (LONG)(x + 30), (LONG)(y + h) };

}

void Missile3::draw()
{
	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, 50, h, memDC, 0, 0, SRCPAINT);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, 50, h, memDC, 0, 0, SRCAND);
}