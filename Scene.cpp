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

HitRecord* Scene::Hit(const Ray& ray, Interval interval) const{
	HitRecord* result = nullptr;

	for(auto obj : objects)
	{
		HitRecord* hit = obj->Hit(ray, interval);
		if(interval.Surrounds(hit->t))
		{
			if(result == nullptr || hit->t < result->t)
				result = hit;
		}
	}
	if(result == nullptr) result = new HitRecord(false);

	return result;
}
