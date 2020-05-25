#include "Object_missile2.h"

void Missile2::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP37));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP38));
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
	attack = 50;
}

void Missile2::update(double time)
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

void Missile2::draw()
{
	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, 50, h, memDC, 0, 0, SRCPAINT);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, 50, h, memDC, 0, 0, SRCAND);
}