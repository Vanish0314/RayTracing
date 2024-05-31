#pragma once

#include <vector>

#include "Hittable.h"
#include "Light.h"

class Scene
{
public:
    std::vector<Hittable*> objects;
    std::vector<Light*> lights;
public:
    Scene();
    ~Scene();
public:
    void AddObject(Hittable* object);
    void AddLight(Light* light);
};
