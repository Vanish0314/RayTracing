/*
 * @Author: Vanish
 * @Date: 2024-06-01 23:45:49
 * @LastEditTime: 2024-06-14 23:37:41
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "RayTracing.h"
#include "Ray.h"
#include "Random.h"

Vector3 Ray::Trace(Interval interval)
{
    if(depth == 0) std::cout <<"\033[34m"<< "[光线开始追踪]" <<"\033[m"<<"==>";

    if(Random::GenerateDouble(0,1) > RUSSIAN_ROULETTE_PROBABILITY) {
        std::cout <<"\033[32m"<< "光线终止追踪：俄罗斯轮盘"<<"\033[m"<<"||||||";
        return Vector3(0, 0, 0);
    }
    HitRecord hitRecord;
    g_Scene->Hit(*this, interval,hitRecord);

    if(!hitRecord.hitted)
    {
        std::cout <<"\033[32m"<< "光线终止追踪：未击中物体"<<"\033[m"<<"||||||";
        return Vector3(0, 0, 0);
    }
    std::cout << "击中物体："<< hitRecord.obj->name << "==>";
     
    this->t = hitRecord.t;

    Vector3 result = hitRecord.material->Shade(*this,hitRecord);

    if(result.x != 0 || result.y != 0 || result.z == 0) std::cout << "光线第{"<<depth--<<"}次弹射结果:"<<"\033[33m"<<"(" <<result.x << "," << result.y << "," << result.z <<")"<<"\033[m"<<"<--";
    if(depth == 0)
    {
        int i = 0;
    }
    return result/RUSSIAN_ROULETTE_PROBABILITY;
}