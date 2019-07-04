#pragma once
#include "Object.h"

class Boss : public Object {
	HBITMAP mask;
	HBITMAP hpEmpty, hpFull, hpMask;
	BITMAP bmp;
	int frame;
	double frame_t;
	double speed;
	bool isShop;
public:
	int hp;
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns);
	virtual void update(double time);
	virtual void draw();

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	double getX() { return x; }
	double getY() { return y; }
	double getW() { return x + 100; }
	double getH() { return y + h; }	
	void setIsShop(bool b) { isShop = b; }

	RECT getCollRect() { return collisionRC; }
};