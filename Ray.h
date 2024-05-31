#pragma once
#include "Vector3.h"
#include "Material.h"

class HitRecord
{
public: 
    bool      hitted;
    Vector3   hitPoint;
    Vector3   normal;
    double    t;
    double    u;
    double    v;
    bool      isFrontFace;
    std::shared_ptr<Material> material;
    

public:
    HitRecord(bool _hitted, Vector3 _hitPoint, Vector3 _normal, double _t, double _u, double _v, Material* _material, bool _isFrontFace):hitted(_hitted),hitPoint(_hitPoint),normal(_normal),t(_t),u(_u),v(_v),material(_material),isFrontFace(_isFrontFace){}
    HitRecord():hitted(false),hitPoint(Vector3(0,0,0)),normal(Vector3(0,0,0)),t(-1),u(-1),v(-1),material(nullptr),isFrontFace(false){}
    ~HitRecord(){}
};

class Interval{
public:
    double tmin = 0, tmax = __FLT_MAX__;
public:
    Interval():tmin(0),tmax(__FLT_MAX__){}
    Interval(double _tmin, double _tmax):tmin(_tmin),tmax(_tmax){}
public:
    bool Surrounds(double t) const { return (tmin <= t && t <= tmax); }
    bool Surrounds(Interval other) const { return (tmin <= other.tmin && other.tmax <= tmax); }
    bool Surrounds(int i) const { return (tmin <= i && i <= tmax); }
};

class Ray
{
public:
    Vector3 origin;
    Vector3 direction;
    double t = -1;//在方向上行进的距离
public:
    Ray():origin(Vector3(0,0,0)),direction(Vector3(0,0,0)){}
    Ray(Vector3 _origin, Vector3 _direction):origin(_origin),direction(_direction){}
    ~Ray(){}
public:
    Vector3 at(double t) const { return origin + direction * t; }
};
