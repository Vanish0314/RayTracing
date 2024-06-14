/*
 * @Author: Vanish
 * @Date: 2024-05-31 23:13:43
 * @LastEditTime: 2024-06-14 20:37:53
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "Color.h"

void Color::Clamp()
{
    if (r < 0) r = 0;
    if (r > 1) r = 1;
    if (g < 0) g = 0;
    if (g > 1) g = 1;
    if (b < 0) b = 0;
    if (b > 1) b = 1;
    if (a < 0) a = 0;
    if (a > 1) a = 1;
}

Vector3 Color::Linear_To_SRGB(Vector3 radiance)
{
    Vector3 srgb = Vector3(1.055 * pow(radiance.x, 1.0 / 2.4) - 0.055,
                           1.055 * pow(radiance.y, 1.0 / 2.4) - 0.055,
                           1.055 * pow(radiance.z, 1.0 / 2.4) - 0.055);

    return srgb;
}

Vector3 Color::ToneMapping_ACES(Vector3 convertedRadiance)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;

    Vector3 color = convertedRadiance;
    Vector3 result = (color * (a * color + b)) / (color * (c * color + d) + e);

    return result;
}

Color Color::GammaCorrection(Vector3 mappedRadiance,double gamma)
{
    Vector3 gammaCorrected = Vector3(pow(mappedRadiance.x, 1.0 / gamma), pow(mappedRadiance.y, 1.0 / gamma), pow(mappedRadiance.z, 1.0 / gamma));

    gammaCorrected.x = std::max(0.0, gammaCorrected.x);
    gammaCorrected.y = std::max(0.0, gammaCorrected.y);
    gammaCorrected.z = std::max(0.0, gammaCorrected.z);
    gammaCorrected.x = std::min(255.0, gammaCorrected.x);
    gammaCorrected.y = std::min(255.0, gammaCorrected.y);
    gammaCorrected.z = std::min(255.0, gammaCorrected.z);

    Color result = Color(gammaCorrected.x, gammaCorrected.y, gammaCorrected.z, 1.0);
    result.Clamp();
    return result ;
}