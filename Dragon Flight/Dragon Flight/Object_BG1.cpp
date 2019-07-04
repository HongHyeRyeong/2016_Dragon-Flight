#include "Object_BG1.h"

void BG1::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP1));
	this->memDC = memDC;
	this->backDC = backDC;
	x = 0;
	y = 0;
}

void BG1::update(double time)
{

}

void BG1::draw()
{
	SelectObject(memDC, resource);
	BitBlt(backDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, memDC, 0, 0, SRCCOPY);
}
