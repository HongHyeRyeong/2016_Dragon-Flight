#pragma once
#include "Object.h"
class BG3 : public Object
{
	int x, y;	//	���� ��ġ

public:
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns);
	virtual void update(double time);
	virtual void draw();
};