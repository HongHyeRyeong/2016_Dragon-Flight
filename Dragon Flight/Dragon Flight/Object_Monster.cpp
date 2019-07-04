#include "Object_Monster.h"

void Monster::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP3));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP10));
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

void Monster::update(double time)
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

	collisionRC = { (LONG)(x), (LONG)y, (LONG)(x + 100), (LONG)(y + 100) };

}

void Monster::draw()
{
	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, w / 4, h, memDC, w / 4 * frame, 0, SRCAND);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, w / 4, h, memDC, w / 4 * frame, 0, SRCPAINT);
}
