#include "Object_Boss.h"

void Boss::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP23));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP27));
	hpEmpty = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP24));
	hpMask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP25));
	hpFull = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP26));
	GetObject(resource, sizeof(BITMAP), &bmp);
	this->memDC = memDC;
	this->backDC = backDC;
	isDead = false;
	w = bmp.bmWidth;
	h = bmp.bmHeight;
	x = 30;
	y = 0 - h;
	hp = 500;
	frame = 0;
	frame_t = 0.f;
	speed = 100.f;
	isShop = false;
	collisionRC = { (LONG)(x), (LONG)y, (LONG)(x + w), (LONG)(y + h) };
}

void Boss::update(double time)
{
	if (y + h < 270 && isShop == false)
		y += speed * time;

	collisionRC = { (LONG)(x), (LONG)y, (LONG)(x + w), (LONG)(y + h) };
}

void Boss::draw()
{
	SetStretchBltMode(backDC, COLORONCOLOR);

	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, x + w, h, memDC, 0, 0, SRCAND);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, x + w, h, memDC, 0, 0, SRCPAINT);

	//	HP바
	SelectObject(memDC, hpMask);
	BitBlt(backDC, 15, 70, 450, 20, memDC, 0, 0, SRCAND);
	SelectObject(memDC, hpEmpty);
	BitBlt(backDC, 15, 70, 450, 20, memDC, 0, 0, SRCPAINT);
	SelectObject(memDC, hpFull);	//	HP / 100 에 따라 출력
	switch (hp)
	{
	case 500:
		StretchBlt(backDC, 15, 70, 450, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 475:
		StretchBlt(backDC, 15, 70, 430, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 450:
		StretchBlt(backDC, 15, 70, 410, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 425:
		StretchBlt(backDC, 15, 70, 390, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 400:
		StretchBlt(backDC, 15, 70, 370, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 375:
		StretchBlt(backDC, 15, 70, 350, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 350:
		StretchBlt(backDC, 15, 70, 330, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 325:
		StretchBlt(backDC, 15, 70, 310, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 300:
		StretchBlt(backDC, 15, 70, 290, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 275:
		StretchBlt(backDC, 15, 70, 270, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;

	case 250:
		StretchBlt(backDC, 15, 70, 250, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 225:
		StretchBlt(backDC, 15, 70, 230, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 200:
		StretchBlt(backDC, 15, 70, 210, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 175:
		StretchBlt(backDC, 15, 70, 190, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 150:
		StretchBlt(backDC, 15, 70, 170, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 125:
		StretchBlt(backDC, 15, 70, 140, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 100:
		StretchBlt(backDC, 15, 70, 110, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 75:
		StretchBlt(backDC, 15, 70, 75, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 50:
		StretchBlt(backDC, 15, 70, 50, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 25:
		StretchBlt(backDC, 15, 70, 25, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;
	case 0:
		StretchBlt(backDC, 15, 70, 0, 20, memDC, 0, 0, 450, 20, SRCCOPY);
		break;


	}
}
