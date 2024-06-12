/*
 * @Author: Vanish
 * @Date: 2024-05-31 03:57:21
 * @LastEditTime: 2024-06-12 17:41:15
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#pragma once

#include <string>
#include <memory>

#include "Ray.h"
#include "Vector3.h"
#include "Material.h"

/// @brief Base class for all hittable objects.
class Hittable {
public:
    std::string               name;
    Vector3                   position;
    std::shared_ptr<Material> material;

public:
    /// @brief 构造函数
    Hittable(std::string name,Vector3 position, std::shared_ptr<Material> material) : name(name), position(position), material(material) {}
public:
    /// @brief 检查是否有交点
    /// @param ray 射出来的光线
    /// @return 返回是否有交点
    virtual void Hit(const Ray& ray ,const Interval interval,HitRecord& hitRecord) = 0;

};