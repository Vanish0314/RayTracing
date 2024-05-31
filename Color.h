#pragma once

/// @brief 范围【0.0, 1.0】的颜色类
/// @details 颜色类，包含红绿蓝和透明度四个分量，范围【0.0, 1.0】
class Color
{
public:
	double r, g, b, a = 1;
public:
	Color(double r = 0, double g = 0, double b = 0, double a = 1) : r(r), g(g), b(b),a(a) {}
	Color(double r = 0, double g = 0, double b = 0) : r(r), g(g), b(b) {}
    ~Color(){};
public:
    void Clamp();
};