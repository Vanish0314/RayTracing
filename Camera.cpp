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
        if(y - progress > 100)
        {
            progress = y;
            progress = progress * 100 / imageHeight;
            std::cout<<"渲染进度:"<<progress<<"%"<<std::endl;
        }
        for(int x=0;x<imageWidth;x++)
        {

            std::shared_ptr<Ray> ray = GetRay(x,y);
            color = PerPixelShading(*ray,scene);
            WritePixelColor(output,color);

            color = Color(0,0,0,1);
            ray.reset();
        }
    }
    std::cout<<"渲染完成"<<std::endl;
}


void Camera::WriteFileHeader(std::ostream& output)
{
    output << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
}


std::shared_ptr<Ray> Camera::GetRay(int pixelIndexX,int pixelIndexY)
{
    Vector3 bias = pixel_00_Location
                 + pixelDeltaX * pixelIndexX
                 - pixelDeltaY * pixelIndexY;
    
    Vector3 origin = m_position;
    Vector3 direction = (bias - origin).Normalized();

    return std::make_shared<Ray>(origin,direction);
}


/// @brief 计算每个像素的颜色
/// @param ray 射出该像素的光线
/// @param scene 场景
/// @param pixelIndex 像素索引
/// @return 返回该像素的颜色
Color Camera::PerPixelShading(Ray& ray,const Scene& scene)
{
    Vector3 radiance;
    //计算辐照度
    switch (mode)
    {
    case RenderMode::PBR:
        radiance = ray.Trace_PBR(Interval(0.0000001f,__FLT_MAX__));
        break;
    case RenderMode::Lambertian:
        radiance = ray.Trace_Lambert(Interval(0.0000001f,__FLT_MAX__));
        break;
    default:
        break;
    }
    

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