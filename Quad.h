#pragma once

#include "Hittable.h"

/// @brief 隐式表达的四边形类；定义position为起始点；u，v为两个非单位方向向量，表示方向与长度；normal存储了法向量
class Quad :public Hittable
{
public:
    Vector3 u,v;
    Vector3 normal;
    double D;
    Vector3 w;

public:
    Quad(std::string name,const Vector3 startPoint,const Vector3 u,const Vector3 v,std::shared_ptr<Material> material):
        Hittable(name,startPoint,material),u(u),v(v)
        {
            auto n = u.Cross(v);
            normal = n.Normalized();
            D = normal.Dot(startPoint); // 计算平面方程的常数项
            w = n/n.Dot(n); // 计算法向量
        }
    ~Quad();
public:
    HitRecord* Hit(const Ray& ray ,const Interval interval) override
    {
        HitRecord* hitRecord = new HitRecord();

        // Ray-Quad intersection algorithm
        // 第零步，平行说明不相交
        float dotProduct = std::abs(ray.direction.Dot(normal));
        if (dotProduct < 1e-6)// 1e-6是个极小数，表示平行的阈值
        {
            hitRecord->hitted = false;
            return hitRecord;
        }
        // 第一步，求包含四边形的平面方程
        double t = (D - ray.origin.Dot(normal)) / dotProduct;
        if(!interval.Surrounds(t)){
            hitRecord->hitted = false;
            return hitRecord;
        }

        // 第二布，判断光线是否与平面相交

        // 第三步，判断交点是否在四边形内
        Vector3 intersectionPoint = ray.at(t);
        isInterior(intersectionPoint,hitRecord);
        if(!hitRecord->hitted)
        {
            return hitRecord;
        }

        // 设置其他信息
        hitRecord->t = t;
        hitRecord->normal = normal;
        hitRecord->hitPoint = intersectionPoint;
        hitRecord->isFrontFace = dotProduct < 0;
        hitRecord->material = material;
        
        return hitRecord;
    }
private:
    void isInterior(const Vector3& point , HitRecord* hitRecord) const
    {
        // 判断点是否在四边形内
        Vector3 planar_HitPoint = point - position;
        double alpha = Vector3::Dot(w,Vector3::Cross(planar_HitPoint,v));
        double beta = Vector3::Dot(w,Vector3::Cross(u,planar_HitPoint));

        Interval interval = Interval(0,1);
        if(!interval.Surrounds(alpha) || !interval.Surrounds(beta)){
            hitRecord->hitted = false;
            return;
        }
        hitRecord->hitted = true;
        hitRecord->u = alpha;
        hitRecord->v = beta;
        return;
    }
};