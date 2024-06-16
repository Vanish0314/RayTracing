/*
 * @Author: Vanish
 * @Date: 2024-06-14 20:45:22
 * @LastEditTime: 2024-06-15 20:44:45
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */

#include <cstdlib>
#include "Random.h"


int Random::GenerateInt(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

double Random::GenerateDouble(double min, double max)
{
    return (double)rand() / RAND_MAX * (max - min) + min;
}

bool Random::consoleOutPutEnable = false;