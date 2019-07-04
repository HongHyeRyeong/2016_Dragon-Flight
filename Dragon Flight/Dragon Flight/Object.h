#pragma once
#pragma comment (lib, "WINMM.lib")
#include <Windows.h>	// WINAPI 함수
#include <stdlib.h>
#include <tchar.h>	// 유니코드
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
	RECT collisionRC;	//	충돌체크 사각형
	bool isDead;

public:
	virtual void init(HDC memDC, HDC backDC, HINSTANCE hIns) = 0;	//	어디서든 호출할 수 있게 초기화 함수를 만들어 줌
	virtual void update(double time) = 0;	//	함수 호출까지 걸리는 시간
	virtual void draw() = 0;	//	memDC -> backDC -> hDC 더블버퍼링
	virtual bool getIsDead() { return isDead; }
	virtual void setDead(bool b) { isDead = b; }
};