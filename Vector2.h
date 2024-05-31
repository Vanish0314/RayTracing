#pragma once

class Vector2
{
private:
    double x = 0, y = 0;
public:
    Vector2(double x = 0, double y = 0):x(x), y(y) {}
    ~Vector2(){};
};