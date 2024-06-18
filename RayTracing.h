/*
 * @Author: Vanish
 * @Date: 2024-05-31 03:56:56
 * @LastEditTime: 2024-06-18 13:46:29
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#pragma once

#define PI 3.1415926535897932
#define SAMPLE_COUNT 2
#define RAY_DEPTH 4
#define RUSSIAN_ROULETTE_PROBABILITY 0.80

//#define DEBUG_TRACERAY
#define CONSOLE_OUTPUT_RENDER_PROGRESS

//#define RENDERMODE_DEBUG
#define RENDERMODE_PBR

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
