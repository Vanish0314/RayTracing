#pragma once

#include <memory>
#include <string>

#include "Vector3.h"
#include "Material.h"
#include "Ray.h"

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
    virtual HitRecord* Hit(const Ray& ray , Interval interval) = 0;
};