#pragma once
#include "Object.h"

class Explosion : public Object
{
	HBITMAP mask;
	BITMAP bmp;
	int frame;
	double frame_t;
public:	
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns);
	virtual void update(double time);
	virtual void draw();
	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void reUse();
};