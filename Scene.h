#pragma once

#include "RayTracing.h"
#include "Hittable.h"
#include "Ray.h"

class Scene
{
public:
    std::vector<Hittable*> objects;// 所有物体
    std::vector<Hittable*> hittables;// 不发光的物体
    std::vector<Hittable*> lights;// 发光物体
public:
    Scene();
    ~Scene();
public:
    void AddObject(Hittable* object);
public:
    HitRecord* Hit(const Ray& ray, Interval interval) const;
};
