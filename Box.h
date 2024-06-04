#pragma once

#include "Hittable.h"

class Box : public Hittable {
public:
    Vector3 topRightCorner;

public:
    Box(std::string name, Vector3 bottomLeftCorner, Vector3 topRightCorner, std::shared_ptr<Material> material):
    Hittable(name,bottomLeftCorner,material), topRightCorner(topRightCorner) {}
};