#pragma once
#include "Object.h"

enum { FLY, ATTACK, HIT };
class Player : public Object {
	HBITMAP mask;
	BITMAP bmp;			//	�̹��� ũ��
	double speed;		//	�ӵ�
	int state;			//	����
	int frame;			//	�ִϸ��̼� ������
	double frame_t;
public:
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns);
	virtual void update(double time);
	virtual void draw();

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	double getX() { return this->x; }
	double getY() { return this->y; }
	double getW() { return x + (w / 4); }
	double getH() { return y + h; }
	RECT getCollRect() { return collisionRC; }
};