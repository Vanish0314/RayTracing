#pragma once

#include "RayTracing.h"
#include "Color.h"
#include "Ray.h"

class PDF{
public:
    static double SampleHemisphere(Vector3 point,std::vector<Ray*>& result,int sampleCount,int rayDepth,Vector3 normal);
};

class Material 
{
public:
    //diffuse color
    Color albedo = Color(0,0,0,1);//diffuse color

    //Emissive color
    bool isEmissive = false; //是否有自发光项
    Vector3 emissiveDistribution = Vector3(0,0,0); //自发光项颜色分布，三项分别对应波长为380nm, 520nm, 650nm的光源（波长我随便写的）
    double emissiveIntensity = 0.0; //emissive intensity

    //Roughness
    double roughness = 0.0; //粗糙度，0表示完全粗糙，1表示完全光滑
    //Metallic
    double metallic = 0.0; //金属度，0表示非金属，1表示金属
public:
    Material(Color albedo, double roughness, double metallic)
        : albedo(albedo), roughness(roughness), metallic(metallic)
    {
        roughness = roughness < 0.0? 0.0 : roughness;
        roughness = roughness > 1.0? 1.0 : roughness;
        metallic = metallic < 0.0? 0.0 : metallic;
        metallic = metallic > 1.0? 1.0 : metallic;

        albedo.Clamp();
    }
    Material(Vector3 emissive, double emissiveIntensity)
        :emissiveDistribution(emissive), emissiveIntensity(emissiveIntensity)
    {
        if(emissiveDistribution.x == 0 && emissiveDistribution.y == 0 && emissiveDistribution.z == 0)
        {
            emissiveDistribution.x = (double)1/3;emissiveDistribution.y =  (double)1/3;emissiveDistribution.z =  (double)1/3;
        }
        else
        {
            double t = emissiveDistribution.x + emissiveDistribution.y + emissiveDistribution.z;
            emissiveDistribution.x = emissiveDistribution.x / t;
            emissiveDistribution.y = emissiveDistribution.y / t;
            emissiveDistribution.z = emissiveDistribution.z / t;
        }
        isEmissive = true;
    }
    ~Material(){}
public:
    Vector3 Shade_Lambert(const Ray& ray,double u,double v,Vector3 normal,Vector3 point);
private:

public:
    
    /// @brief 计算光线与材质的交点处的颜色,基于物理的渲染模型,使用Cook-Torrance BRDF模型,PDF为常数
    /// @param x 世界坐标系下的光线交点
    /// @param wo 入射光线方向
    /// @param sampleCount PDF采样次数
    /// @param r 光线传播距离
    /// @return 光线起点在wo方向上的inradiance
    Vector3 Shade_PBS(const Ray& ray,int sampleCount,double u,double v,Vector3 normal);

    /// @brief 渲染方程中的自发光项
    /// @param x 世界坐标系下的光线交点
    /// @param wo 
    /// @return 
    Vector3 EmissiveTerm(Vector3 x, Vector3 wo);

    /// @brief 反射方程
    /// @param x 世界坐标系下的光线交点
    /// @param wo 入射光线方向
    /// @param sampleCount PDF采样次数
    /// @return 反射方程的解
    Vector3 ReflectionTerm(Vector3 x, Vector3 wo,Vector3 normal, int sampleCount,int rayDepth);

private:
    double BRDF(Vector3 x, Vector3 wo, Vector3 wi,Vector3 normal);
    double NormalDistribution_GGX(Vector3 wi, Vector3 wo,Vector3 normal);
    double FresnelTerm_Schlick(Vector3 wi,Vector3 normal);
    double GeometryOcclusionTerm_Schlick(Vector3 wo,Vector3 wi,Vector3 normal);
};
