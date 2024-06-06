#include "Ray.h"

Vector3 Ray::Trace(Interval interval)
{
    if(depth == 0) return Vector3(0, 0, 0);

    HitRecord* hitRecord;
    hitRecord = g_Scene->Hit(*this, interval);

    if(!hitRecord->hitted) return Vector3(0, 0, 0);

    this->t = hitRecord->t;

    Vector3 result = hitRecord->material->Shade(*this,*hitRecord);

    return result;
}