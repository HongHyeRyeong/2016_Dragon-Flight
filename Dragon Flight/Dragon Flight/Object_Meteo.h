#pragma once
#include "Object.h"

class Meteo : public Object
{
	HBITMAP mask;
	BITMAP bmp;
	double speed, frame_t;
	bool isActive;
public:
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns);
	virtual void update(double time);
	virtual void draw();

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setDead(bool b) { isDead = b; }
	double getX() { return x; }
	double getY() { return y; }
	double getW() { return x + w; }
	double getH() { return y + h; }
	bool getIsActive() { return isActive; }
	void setActive(bool b) { isActive = b; }
	RECT getCollRect() { return collisionRC; }
};