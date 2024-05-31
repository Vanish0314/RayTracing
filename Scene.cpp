#include "Scene.h"
Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddObject(Hittable* obj)
{
	objects.push_back(obj);
}

void Scene::AddLight(Light* light)
{
	lights.push_back(light);
}

