#pragma once

#include "Hittable.h"
#include "Vector3.h"

class Sphere:public Hittable
{
public:
    float radius;
public:
    Sphere(std::string name,float r,Vector3 position, std::shared_ptr<Material> material):Hittable(name,position,material),radius(r) {}
    ~Sphere();
public:

HitRecord* Hit (const Ray& ray , Interval interval)
    override {
        //Create a hit record to store the hit information
        HitRecord* result = new HitRecord();

        // Solve t^2 * d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
        // where o: sphere center, p: ray origin, d: ray direction, R: sphere radius,t: 光线行进距离
        // a = d.d
        //b = 2*(o-p).d 
        //c = (o-p).(o-p)-R^2
        Vector3 oc = position - ray.origin; // o-p
        float a = ray.direction.Dot(ray.direction); // d.d
        float b = 2.0f * oc.Dot(ray.direction); // 2*(o-p).d
        float c = oc.Dot(oc) - radius * radius; // (o-p).(o-p)-R^2

        // Calculate the discriminant
        // discriminant = b^2 - 4ac
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            result->hitted = false;
            return result;
        }
        result->hitted = true;
        
        // Calculate the two possible intersection points
        // t = (-b +- sqrt(discriminant)) / (2*a)
        float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        if (interval.Surrounds(std::min(t1, t2))) return result;
        result->t = std::min(t1, t2);

        //Configure the hit record
        result->hitPoint = ray.at(result->t);
        result->normal = (result->hitPoint - position) / radius;
        result->material = material;
        result->isFrontFace = (ray.direction.Dot(result->normal) < 0);
        result->u = GetUAt(result->hitPoint);
        result->v = GetVAt(result->hitPoint);

        return result;
    }
private:
    double GetUAt(const Vector3& hitPoint) const;
    double GetVAt(const Vector3& hitPoint) const;
};