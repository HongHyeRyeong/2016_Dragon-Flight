#include "Object_missile.h"

void Missile::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP2));
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
	attack = 25;
}

void Missile::update(double time)
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

void Missile::draw()
{
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, 30, h, memDC, 30, 0, SRCAND);
	BitBlt(backDC, x, y, 30, h, memDC, 0, 0, SRCPAINT);
}
