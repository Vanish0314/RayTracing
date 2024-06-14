/*
 * @Author: Vanish
 * @Date: 2024-06-04 21:42:34
 * @LastEditTime: 2024-06-14 21:01:16
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#pragma once

#include "Hittable.h"

class Box : public Hittable {
public:
    Vector3 topRightCorner;

public:
    Box(std::string name, Vector3 bottomLeftCorner, Vector3 topRightCorner, std::shared_ptr<Material> material):
    Hittable(name,bottomLeftCorner,material), topRightCorner(topRightCorner) {}
};