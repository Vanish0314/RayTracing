#pragma once

#include <cmath>

class Vector3
{
public:
    double x =0, y = 0, z = 0;
public:
    Vector3(double x = 0, double y = 0, double z = 0): x(x), y(y), z(z){}
    ~Vector3(){};
public:
    Vector3 operator+(const Vector3& other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 operator-(const Vector3& other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 operator*(const double& scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector3 operator/(const double& scalar) const
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    double Magnitude() const
    {
        return sqrt(x*x + y*y + z*z);
    }
    void Normalize()
    {
        double mag = Magnitude();
        if (mag > 0)
        {
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }
    Vector3 Normalized() const
    {
        Vector3 v = *this;
        v.Normalize();
        return v;
    }

    double Dot(const Vector3& other) const
    {
        return x*other.x + y*other.y + z*other.z;
    }

    Vector3 Cross(const Vector3& other) const
    {
        return Vector3(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
    }
public:
    static double Dot(const Vector3& a, const Vector3& b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    static Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }
    

public:
    static double Distance(const Vector3& a, const Vector3& b)
    {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        double dz = a.z - b.z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }

    static double Angle(const Vector3& a, const Vector3& b)
    {
        double dot = a.Dot(b);
        double magA = a.Magnitude();
        double magB = b.Magnitude();
        if (magA > 0 && magB > 0)
        {
            return acos(dot / (magA * magB));
        }
        return 0;
    }


    static Vector3 Reflect(const Vector3& incident, const Vector3& normal)
    {
        return incident - normal * 2 * normal.Dot(incident);
    }

    static Vector3 Refract(const Vector3& incident, const Vector3& normal, double eta)
    {
        double dot = normal.Dot(incident);
        double k = 1 - eta * eta * (1 - dot * dot);
        if (k < 0)
        {
            return Vector3(0, 0, 0);
        }
        return  incident * eta + normal * (eta * dot - sqrt(k));
    }

    static Vector3 Random()
    {
        return Vector3(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX, rand() / (double)RAND_MAX);
    }


    static Vector3 RandomUnit()
    {
        Vector3 v = Random();
        v.Normalize();
        return v;
    }

};

