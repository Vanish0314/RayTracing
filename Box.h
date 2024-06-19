/*
 * @Author: Vanish
 * @Date: 2024-06-04 21:42:34
 * @LastEditTime: 2024-06-18 19:34:25
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#pragma once

#include "Hittable.h"

class Box : public Hittable {
public:
    Vector3 m_BottomLeftCorner;
    Vector3 m_TopRightCorner;

public:
    Box(std::string name, Vector3 bottomLeftCorner, Vector3 topRightCorner, std::shared_ptr<Material> material):
    Hittable(name,(bottomLeftCorner + topRightCorner)/2,material), m_BottomLeftCorner(bottomLeftCorner), m_TopRightCorner(topRightCorner) {}

public:
    void Hit(const Ray& ray ,const Interval interval,HitRecord& hitRecord) override
    {
        // check if ray could intersect with the box
        Vector3 inDir = ray.direction;
        Vector3 centerToRay = ray.origin - position;
        float distanceMin = centerToRay.Magnitude() * Vector3::Dot(inDir, Vector3(0,0,0)- centerToRay);
        float halfWidth = (m_TopRightCorner - m_BottomLeftCorner).x/ 2;
        float halfHeight = (m_TopRightCorner - m_BottomLeftCorner).y / 2;
        float halfDepth = (m_TopRightCorner - m_BottomLeftCorner).z / 2;
        if(distanceMin > halfWidth && distanceMin > halfHeight && distanceMin > halfDepth) 
        {
            hitRecord.hitted = false;
            return;
        }
    }

public:
    void Rotate(const Vector3& elueAngle);
};