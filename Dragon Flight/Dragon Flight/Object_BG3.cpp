#include "Object_BG3.h"

void BG3::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP7));
	this->memDC = memDC;
	this->backDC = backDC;
	x = 0;
	y = 0;
}

void BG3::update(double time)
{
}

void BG3::draw()
{
	SelectObject(memDC, resource);
	BitBlt(backDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, memDC, 0, 0, SRCCOPY);
}
