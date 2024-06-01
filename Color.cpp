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

Vector3 Color::ToneMapping_Reinhard(Vector3 convertedRadiance)
{
    float luminance = 0.2126 * convertedRadiance.x + 0.7152 * convertedRadiance.y + 0.0722 * convertedRadiance.z;
    float scale = 1.0 / (1.0 + luminance);

    return Vector3(scale * convertedRadiance.x, scale * convertedRadiance.y, scale * convertedRadiance.z);
}

Color Color::GammaCorrection(Vector3 mappedRadiance,double gamma)
{
    Vector3 gammaCorrected = Vector3(pow(mappedRadiance.x, 1.0 / gamma), pow(mappedRadiance.y, 1.0 / gamma), pow(mappedRadiance.z, 1.0 / gamma));

    return Color(gammaCorrected.x, gammaCorrected.y, gammaCorrected.z, 1.0);
}