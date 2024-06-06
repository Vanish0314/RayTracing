#pragma once

#include "Vector3.h"

/// @brief 范围【0.0, 1.0】的颜色类
/// @details 颜色类，包含红绿蓝和透明度四个分量，范围【0.0, 1.0】
class Color
{
public:
    double r, g, b, a;
public:
	Color() : r(0), g(0), b(0), a(1) {}
    Color(double r, double g, double b, double a = 1) : r(r), g(g), b(b), a(a) {}
    Color(double r, double g, double b) : r(r), g(g), b(b), a(1) {}
    ~Color() {}
public:
    void Clamp();
	static Vector3 Linear_To_SRGB(Vector3 radiance);
 	static Vector3 ToneMapping_ACES(Vector3 convertedRadiance);
	static Color GammaCorrection(Vector3 mappedRadiance, double gamma);
};
