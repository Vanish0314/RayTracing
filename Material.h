#pragma once

#include "Color.h"

class Material 
{
public:
    Color albedo = Color(0,0,0,1);

public:
    Material(Color albedo)
        : albedo(albedo)
    {}
};