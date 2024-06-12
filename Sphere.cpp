/*
 * @Author: Vanish
 * @Date: 2024-05-31 22:47:58
 * @LastEditTime: 2024-06-12 18:45:26
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "Sphere.h"

double Sphere::GetUAt(const Vector3& hitPoint) const {
    return 0.5;
    double theta = std::acos(hitPoint.z / radius);
    double phi = std::atan2(hitPoint.y, hitPoint.x);
    return (phi + 3.1415926535897932) / (2 * 3.1415926535897932);
}

double Sphere::GetVAt(const Vector3& hitPoint) const {
    return 0.5;
    double theta = std::acos(hitPoint.z / radius);
    return (theta + 3.1415926535897932 / 2) / 3.1415926535897932;
}