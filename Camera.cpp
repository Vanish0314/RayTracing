/*
 * @Author: Vanish
 * @Date: 2024-05-31 03:57:26
 * @LastEditTime: 2024-06-14 23:37:30
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "Camera.h"


void Camera::Render(const Scene& scene,std::ostream& output)
{
    //写入ppm文件头
    WriteFileHeader(output);
    
    Color color = Color(0,0,0,1);

    double progress = 0;
    //逐像素渲染,从左到右，从上到下
    for(int y=0;y<imageHeight;y++)
    {
        // //输出进度
        // if(y - progress > 100)
        // {
        //     progress = y;
        //     progress = progress * 100 / imageHeight;
        //     std::cout<<"渲染进度:"<<progress<<"%"<<std::endl;
        // }
        
        for(int x=0;x<imageWidth;x++)
        {
            //渲染每个像素
            for(int i=0;i<samplesPerPixel;i++)
            {
                Ray ray = Ray(Vector3(0,0,0),Vector3(0,0,0));
                GetRay(x,y,ray);

                color += PerPixelShading(ray,scene);
                std::cout<<"\033[31m"<<"此光线像素结果:【"<<color.r<<","<<color.g<<","<<color.b<<"】"<<"\033[m"<<"/////==/////";
            }
            color /= samplesPerPixel;
            color.Clamp();

            std::cout<<"\033[31m"<<"像素最终结果:【"<<color.r<<","<<color.g<<","<<color.b<<"】"<<"\033[m"<<std::endl;
            WritePixelColor(output,color);
            color = Color(0,0,0,1);
        }
    }
    std::cout<<"渲染完成"<<std::endl;
}


void Camera::WriteFileHeader(std::ostream& output)
{
    output << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
}


void Camera::GetRay(int pixelIndexX,int pixelIndexY,Ray& ray)
{
    Vector3 bias = pixel_00_Location
                 + pixelDeltaX * pixelIndexX
                 - pixelDeltaY * pixelIndexY;
    
    Vector3 origin = m_position;
    Vector3 direction = (bias - origin).Normalized();

    ray = Ray(origin,direction);
}


/// @brief 计算每个像素的颜色
/// @param ray 射出该像素的光线
/// @param scene 场景
/// @param pixelIndex 像素索引
/// @return 返回该像素的颜色
Color Camera::PerPixelShading(Ray& ray,const Scene& scene)
{
    //计算辐照度
    Vector3 radiance;
    radiance = ray.Trace(Interval());

    //辐照度颜色化

    //Step1:映射到颜色空间，由于暂时没有定义辐照度的光谱采样情况，这一步可以掠过(实际上什么都没干)
    Vector3 convertedRadiance = Color:: Linear_To_SRGB(radiance);
    //Step2:色调映射
    Vector3 mappedRadiance = Color::ToneMapping_ACES(radiance);
    //step3:伽马矫正
    Color gammaCorrectedColor = Color::GammaCorrection(radiance,2.2f);

    //返回颜色
    return gammaCorrectedColor;
}


void Camera::WritePixelColor(std::ostream& output,Color& color)
{
    color.Clamp();
    output << (int)(color.r*255) << " " << (int)(color.g*255) << " " << (int)(color.b*255) << "\n";
}