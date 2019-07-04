#pragma once
#pragma comment (lib, "WINMM.lib")
#include <Windows.h>	// WINAPI �Լ�
#include <stdlib.h>
#include <tchar.h>	// �����ڵ�
#include <time.h>
#include "resource.h"
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 800
#define MAX_MONSTER 5
#define MAX_MISSILE 100

class Object {
protected:
	double x, y, w, h;
	HDC memDC, backDC;
	HBITMAP resource;
	RECT collisionRC;	//	�浹üũ �簢��
	bool isDead;

public:
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns) = 0;	//	��𼭵� ȣ���� �� �ְ� �ʱ�ȭ �Լ��� ����� ��
	virtual void update(double time) = 0;	//	�Լ� ȣ����� �ɸ��� �ð�
	virtual void draw() = 0;	//	memDC -> backDC -> hDC ������۸�
	virtual bool getIsDead() { return isDead; }
	virtual void setDead(bool b) { isDead = b; }
};