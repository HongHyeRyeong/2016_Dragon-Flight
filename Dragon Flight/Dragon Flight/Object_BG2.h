#pragma once

#include "Object.h"
class BG2 : public Object
{
	int x, y;	//	현재 위치

public:
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns);
	virtual void update(double time);
	virtual void draw();
};