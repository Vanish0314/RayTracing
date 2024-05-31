#pragma once

#include "Hittable.h"

class Light : public Hittable
{
public:
    Light(int id, int x, int y, int r);
    int getId() const;
    int getX() const;
    int getY() const;
    int getR() const;
    void setX(int x);
    void setY(int y);
    void setR(int r);

private:
    int id;
    int x;
    int y;
    int r;
};