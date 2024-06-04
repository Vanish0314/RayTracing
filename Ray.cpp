#include "Ray.h"

Vector3 Ray::Trace_PBR(Interval interval)
{
    if(depth == 0) return Vector3(0, 0, 0);

    HitRecord* hitRecord;
    hitRecord = g_Scene->Hit(*this, interval);

    if(!hitRecord->hitted) return Vector3(0, 0, 0);

    this->t = hitRecord->t;

    Vector3 result = hitRecord->material->Shade_PBS(*this,PDF_SAMPLE_COUNT,hitRecord->u,hitRecord->v,hitRecord->normal);
    return result;
}

Vector3 Ray::Trace_Lambert(Interval interval)
{
    if(depth == 0) return Vector3(0, 0, 0);

    HitRecord* hitRecord;
    hitRecord = g_Scene->Hit(*this, interval);

    if(!hitRecord->hitted) return Vector3(0, 0, 0);

    this->t = hitRecord->t;

    Vector3 result = hitRecord->material->Shade_Lambert(*this,hitRecord->u,hitRecord->v,hitRecord->normal,hitRecord->hitPoint);
    return result;
}   