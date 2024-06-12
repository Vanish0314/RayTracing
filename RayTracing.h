#pragma once

#define PI 3.1415926535897932
#define PDF_SAMPLE_COUNT 2
#define RAY_DEPTH 6

#include <cmath>
#include <random>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

// #include "Vector3.h"
// #include "Camera.h"
// #include "Sphere.h
// #include "Quad.h"
// #include "Light.h"
// #include "Material.h"

// 前向声明
class Scene;
class Ray;
class Material;
class HitRecord;
class Interval;
class Hittable;


// 定义全局变量
extern Scene* g_Scene;