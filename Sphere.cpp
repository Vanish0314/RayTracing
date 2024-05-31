#include "Sphere.h"

double Sphere::GetUAt(const Vector3& hitPoint) const {
    double theta = std::acos(hitPoint.z / radius);
    double phi = std::atan2(hitPoint.y, hitPoint.x);
    return (phi + 3.1415926535897932) / (2 * 3.1415926535897932);
}

double Sphere::GetVAt(const Vector3& hitPoint) const {
    double theta = std::acos(hitPoint.z / radius);
    return (theta + 3.1415926535897932 / 2) / 3.1415926535897932;
}