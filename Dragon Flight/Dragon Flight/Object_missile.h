#pragma once
#include "Object.h"

class Missile : public Object {
	BITMAP bmp;
	double speed, frame_t;
	bool isActive;
public:
	int attack;		//	ÇÇÇØ·®
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns);
	virtual void update(double time);
	virtual void draw();

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	double getX() { return x; }
	double getY() { return y; }
	double getW() { return x + 30; }
	double getH() { return y + h; }
	bool getIsActive() { return isActive; }
	void setActive(bool b) { isActive = b; }
	RECT getCollRect() { return collisionRC; }

};