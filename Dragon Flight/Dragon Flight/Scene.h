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
	void run(double time);		//	����Ʈ�� ��� Object ��ü���� update(), draw() �Լ��� ȣ����
};