/*
 * @Author: Vanish
 * @Date: 2024-05-31 03:57:26
 * @LastEditTime: 2024-06-19 10:26:07
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "Camera.h"

void Camera::Render(const Scene &scene, std::ostream &output)
{
    // 写入ppm文件头
    WriteFileHeader(output);

    Color color = Color(0, 0, 0, 1);
    Vector3 radiance = Vector3(0, 0, 0);

    double progress = 0;
    // 逐像素渲染,从左到右，从上到下
    for (int y = 0; y < imageHeight; y++)
    {
#ifdef CONSOLE_OUTPUT_RENDER_PROGRESS
        //输出进度
        if(y - progress > imageHeight / 100)
        {
            progress = y;
            progress = progress * 100 / imageHeight;
            std::cout<<"渲染进度:"<<progress<<"%"<<std::endl;
            progress = y;
        }
#endif

        for (int x = 0; x < imageWidth; x++)
        {
#ifdef DEBUG_TRACERAY
                Random::consoleOutPutEnable = Random::GenerateDouble(0,1) < 0.000001;
#endif
            // 渲染每个像素
            for (int i = 0; i < samplesPerPixel; i++)
            {

                Ray ray = Ray(Vector3(0, 0, 0), Vector3(0, 0, 0));
                GetRay(x, y, ray);

                radiance += ray.Trace(Interval());
#ifdef DEBUG_TRACERAY
                if(Random::consoleOutPutEnable)
                    std::cout << "\033[31m" <<i+1<<"/"<<samplesPerPixel<<"光线像素结果:【" << radiance.x << "," << radiance.y << "," << radiance.z << "】" << "\033[m" << "/////==/////";
#endif
            }
            radiance = radiance / samplesPerPixel;
            color = RadianceToColor(radiance);
#ifdef DEBUG_TRACERAY
            if(Random::consoleOutPutEnable)
                std::cout << "\033[31m" << "像素最终结果:【" << color.r << "," << color.g << "," << color.b << "】" << "\033[m" << std::endl;
#endif
            WritePixelColor(output, color);
            color = Color(0, 0, 0, 1);
            radiance = Vector3(0, 0, 0);
        }
    }
    std::cout << "渲染完成" << std::endl;
}

void Camera::WriteFileHeader(std::ostream &output)
{
    output << "P3\n"
           << imageWidth << " " << imageHeight << "\n255\n";
}

void Camera::GetRay(int pixelIndexX, int pixelIndexY, Ray &ray)
{
    Vector3 bias = pixel_00_Location + pixelDeltaX * pixelIndexX - pixelDeltaY * pixelIndexY;
    bias += pixelDeltaX * 0.49 * Random::GenerateDouble(-1, 1) - pixelDeltaY * 0.49 * Random::GenerateDouble(-1, 1);

    Vector3 origin    = m_position;
    Vector3 direction = (bias - origin).Normalized();

    ray = Ray(origin, direction);
}

/// @brief 计算每个像素的颜色
/// @param ray 射出该像素的光线
/// @param scene 场景
/// @param pixelIndex 像素索引
/// @return 返回该像素的颜色
Color Camera::RadianceToColor(Vector3 radiance)
{
    // // 计算辐照度
    // Vector3 radiance;
    // radiance = ray.Trace(Interval());

    // // 辐照度颜色化
    //Step1: 将SPD转换到XYZ空间 这里不做变换
    Vector3 XYZ = radiance;
    //Step2: XYZ TO RGB
    Vector3 RGB = Color::XYZToRGB(XYZ);
    //Step3: 色调映射
    Vector3 TomeMapped = Color::ToneMapping_ACES(RGB);
    //Step4: 伽马校正
    Color GammaCorrected = Color::GammaCorrection(RGB, 1.0 / 2.2);
    
    return GammaCorrected;
    
}

void Camera::WritePixelColor(std::ostream &output, Color &color)
{
    if(color.r == 0 && color.g == 0 && color.b == 0)
    {
        Vector3 black = Vector3(0, 0, 0);
    }
    
    color.Clamp();
    output << (int)(color.r * 255) << " " << (int)(color.g * 255) << " " << (int)(color.b * 255) << "\n";
}