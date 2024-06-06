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
    Color albedo; //diffuse color

    bool isEmissive = false; //是否有自发光项
    Vector3 emissiveDistribution = Vector3(0,0,0); //自发光项颜色分布，三项分别对应波长为380nm, 520nm, 650nm的光源（波长我随便写的）
    double emissiveIntensity = 0.0; //emissive intensity
public:
    Material(Color albedo): albedo(albedo) 
    {
        isEmissive = false;
        albedo.Clamp();
    }
    Material(Vector3 emissiveDistribution,double emissiveIntensity): emissiveDistribution(emissiveDistribution) , emissiveIntensity(emissiveIntensity)
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
    /// @brief 计算着色
    /// @param ray_In 射入光线
    /// @param hitRecord 与材质的相交信息
    /// @param scatterRecord 散射的信息与设置
    /// @return 返回wo方向上计算了光照衰减（根据t）后的radiance
    virtual Vector3 Shade(Ray& ray_In, HitRecord& hitRecord) = 0;
    /// @brief 计算自发光项
    /// @param wo 自发光radiance的方向
    /// @param point 自发光radiance的位置
    /// @return 返回wo方向上计算了光照衰减（根据t）后的radiance
    virtual Vector3 EmissiveTerm(Vector3 wo , Vector3 point) = 0;
    /// @brief 计算反射项
    /// @param ray_In 射入光线
    /// @param hitRecord 与材质的相交信息
    /// @param scatterRecord 散射的信息与设置
    /// @return 返回wo方向上计算了光照衰减（根据t）后的radiance
    virtual Vector3 ReflectionTerm(Ray& ray_In, HitRecord& hitRecord) = 0;
};

class Material_Lambert : public Material
{
public:
    double kd = 0.0; //漫反射系数
public:
    Material_Lambert(Color albedo, double kd)
        : Material(albedo), kd(kd)
    {
        kd = kd < 0.0? 0.0 : kd;
        kd = kd > 1.0 ? 1.0 : kd;
        albedo.Clamp();
    }
    Material_Lambert(Vector3 emissiveDistribution, double emissiveIntensity)
        : Material(emissiveDistribution, emissiveIntensity)
    {
        isEmissive = true;
    }
    ~Material_Lambert() {}
public:
    Vector3 Shade(Ray& ray_In, HitRecord& hitRecord) override;
    Vector3 EmissiveTerm(Vector3 wo , Vector3 point) override;
    Vector3 ReflectionTerm(Ray& ray_In, HitRecord& hitRecord) override;
};

class Material_PBM : public Material
{
public:
    double roughness = 0.0; //粗糙度，0表示完全粗糙，1表示完全光滑
    double metallic = 0.0; //金属度，0表示非金属，1表示金属

public:
    Material_PBM(Color albedo, double roughness, double metallic)
        : Material(albedo), roughness(roughness), metallic(metallic)
    {
        roughness = roughness < 0.0? 0.0 : roughness;
        roughness = roughness > 1.0? 1.0 : roughness;
        metallic = metallic < 0.0? 0.0 : metallic;
        metallic = metallic > 1.0? 1.0 : metallic;

        albedo.Clamp();
    }
    Material_PBM(Vector3 emissiveDistribution, double emissiveIntensity)
        : Material(emissiveDistribution, emissiveIntensity)
    {
        isEmissive = true;
    }
    ~Material_PBM() {}

public:
    Vector3 Shade(Ray& ray_In, HitRecord& hitRecord) override;
    Vector3 EmissiveTerm(Vector3 wo , Vector3 point) override;
    Vector3 ReflectionTerm(Ray& ray_In, HitRecord& hitRecord) override;

private:
    double BRDF(Vector3 x, Vector3 wo, Vector3 wi,Vector3 normal);
    double NormalDistribution_GGX(Vector3 wi, Vector3 wo,Vector3 normal);
    double FresnelTerm_Schlick(Vector3 wi,Vector3 normal);
    double GeometryOcclusionTerm_Schlick(Vector3 wo,Vector3 wi,Vector3 normal);
};
