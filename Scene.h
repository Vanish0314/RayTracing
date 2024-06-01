#pragma once

#include "RayTracing.h"
#include "Hittable.h"
#include "Ray.h"

class Scene
{
public:
    std::vector<Hittable*> objects;
public:
    Scene();
    ~Scene();
public:
    void AddObject(Hittable* object);
public:
    HitRecord* Hit(const Ray& ray, Interval interval) const;
};
