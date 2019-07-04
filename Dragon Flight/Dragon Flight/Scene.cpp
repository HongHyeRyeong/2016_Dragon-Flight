#include "Scene.h"

void Scene::addObject(Object* obj)
{
	for (int i = 0; i < MAX; ++i)
	{
		if (ObjectList[i] != NULL)
			continue;
		ObjectList[i] = obj;
		break;
	}
}

void Scene::run(double time)
{
	for (int i = 0; i < MAX; ++i)
	{
		if (ObjectList[i] == NULL)	continue;
		if (ObjectList[i]->getIsDead() == true)
		{
			ObjectList[i] = NULL;
			continue;
		}
		if(!shop)
			ObjectList[i]->update(time);
		ObjectList[i]->draw();
	}
}
