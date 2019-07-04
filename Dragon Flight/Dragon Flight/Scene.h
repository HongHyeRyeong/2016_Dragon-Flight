#pragma once
#include "Object.h"
#define MAX 500
class Scene
{
	Object* ObjectList[500];
public:
	bool shop = false;
	Scene() {
		memset(ObjectList, NULL, sizeof(ObjectList));
	}
	void addObject(Object* obj);
	void run(double time);		//	리스트에 담긴 Object 객체들의 update(), draw() 함수를 호출함
};