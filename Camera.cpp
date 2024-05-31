#include "Camera.h"


void Camera::Render(const Scene& scene,std::ostream& output)
{
    //写入ppm文件头
    WriteFileHeader(output);
    
    Color color = Color(0,0,0,1);
    //逐像素渲染,从左到右，从上到下
    for(int y=0;y<imageHeight;y++)
    {

        //输出进度
        std::cout<<"正在渲染第"<<y+1<<"行剩余"<<imageHeight-y-1<<"行..."<<std::endl;

        for(int x=0;x<imageWidth;x++)
        {

            std::shared_ptr<Ray> ray = GetRay(x,y);
            color = PerPixelShading(*ray,scene);
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
Color Camera::PerPixelShading(const Ray& ray,const Scene& scene)
{
    Hittable* nearestObject = nullptr;
    double nearestT = -1;
    //找到最近的相交对象
    for(auto& obj : scene.objects)
    {
        HitRecord* hitRecord = obj->Hit(ray,Interval());
        if(hitRecord->hitted)
        {
            if(nearestObject == nullptr || hitRecord->t < nearestT)
            {
                nearestObject = obj;
                nearestT = hitRecord->t;
            }
        }
    }

    //如果没有相交对象，则返回背景颜色
    if(nearestObject == nullptr)
    {
        return Color(0,0,0,1);
    }

    //计算该像素的颜色
    Color color = nearestObject->material.get()->albedo;
    return color;
}


void Camera::WritePixelColor(std::ostream& output,Color& color)
{
    color.Clamp();
    output << (int)(color.r*255) << " " << (int)(color.g*255) << " " << (int)(color.b*255) << "\n";
}