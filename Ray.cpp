/*
 * @Author: Vanish
 * @Date: 2024-06-01 23:45:49
 * @LastEditTime: 2024-06-12 18:42:53
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "Ray.h"

Vector3 Ray::Trace(Interval interval)
{
    if(depth == 0) return Vector3(0, 0, 0);

    HitRecord hitRecord;
    g_Scene->Hit(*this, interval,hitRecord);

    if(!hitRecord.hitted) return Vector3(0, 0, 0);

    this->t = hitRecord.t;

    Vector3 result = hitRecord.material->Shade(*this,hitRecord);

    return result;
}