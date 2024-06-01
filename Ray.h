#pragma once


#include <memory>

#include "Vector3.h"
#include "Material.h"
#include "Scene.h"

class HitRecord
{
public: 
    bool      hitted; //是否击中物体
    Vector3   hitPoint; //击中点
    Vector3   normal; //法线
    double    t;//击中距离
    double    u;//纹理坐标u
    double    v;//纹理坐标v
    bool      isFrontFace;//是否是正面
    std::shared_ptr<Material> material;//材质
    

public:
    HitRecord(bool hitted):hitted(hitted),hitPoint(Vector3(0,0,0)),normal(Vector3(0,0,0)),t(-1),u(-1),v(-1),isFrontFace(false),material(nullptr){}
    HitRecord(bool _hitted, Vector3 _hitPoint, Vector3 _normal, double _t, double _u, double _v, Material* _material, bool _isFrontFace):hitted(_hitted),hitPoint(_hitPoint),normal(_normal),t(_t),u(_u),v(_v),isFrontFace(_isFrontFace),material(_material){}
    HitRecord():hitted(false),hitPoint(Vector3(0,0,0)),normal(Vector3(0,0,0)),t(-1),u(-1),v(-1),isFrontFace(false),material(nullptr){}
    ~HitRecord(){}
};

class Interval{
public:
    double tmin = 0, tmax = __FLT_MAX__;
public:
    Interval():tmin(0),tmax(__FLT_MAX__){}
    Interval(double _tmin, double _tmax):tmin(_tmin),tmax(_tmax){}
    Interval(double tmin):tmin(tmin),tmax(__FLT_MAX__){}
public:
    bool Surrounds(double t) const { return (tmin <= t && t <= tmax); }
    bool Surrounds(Interval other) const { return (tmin <= other.tmin && other.tmax <= tmax); }
    bool Surrounds(int i) const { return (tmin <= i && i <= tmax); }
};

class Ray
{
public:
    int     depth       = RAY_DEPTH; //光线的递归深度,在逐像素渲染函数中修改
    Vector3 origin;
    Vector3 direction;
    double  t           = -1;//在方向上行进的距离
public:
    Ray():origin(Vector3(0,0,0)),direction(Vector3(0,0,0)){}
    Ray(Vector3 _origin, Vector3 _direction):origin(_origin),direction(_direction){}
    ~Ray(){}
public:
    Vector3 at(double t) const { return origin + direction * t; }
public:
    /// @brief 返回光线结果
    /// @param interval 
    /// @return 
    Vector3 Trace(Interval interval);
};
