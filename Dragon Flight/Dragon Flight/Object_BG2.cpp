#include "Object_BG2.h"

void BG2::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP6));
	this->memDC = memDC;
	this->backDC = backDC;
	x = 0;
	y = 0;
}

void BG2::update(double time)
{
}

void BG2::draw()
{
	SelectObject(memDC, resource);
	BitBlt(backDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, memDC, 0, 0, SRCCOPY);
}
