#include "Object_Explosion.h"

void Explosion::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP33));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP34));	
	this->memDC = memDC;
	this->backDC = backDC;
	GetObject(resource, sizeof(BITMAP), &bmp);
	x = 0;
	y = 0;
	w = bmp.bmWidth;
	h = bmp.bmHeight;

	frame = 0;
	isDead = false;
}

void Explosion::update(double time)
{
	frame_t += time;
	if (frame_t > 0.1f)
	{
		frame_t = 0;
		frame++;
		if (frame > 5)
			isDead = true;
	}

}

void Explosion::draw()
{
	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, w / 5, h, memDC, w / 5 * frame, 0, SRCAND);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, w / 5, h, memDC, w / 5 * frame, 0, SRCPAINT);
}

void Explosion::reUse()
{
	isDead = false;
	frame = 0;
	frame_t = 0;
}
