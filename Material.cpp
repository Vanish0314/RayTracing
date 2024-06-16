/*
 * @Author: Vanish
 * @Date: 2024-06-02 04:28:38
 * @LastEditTime: 2024-06-16 21:46:38
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "Material.h"


double PDF::SampleHemisphere(Vector3 point,Ray& result,Vector3 normal)
{

    Vector3 randomVec = Vector3::RandomInHemisphere(normal);
    result.origin = point + normal * 0.00001;
    result.direction = randomVec.Normalized();
    result.depth++;
    result.t = 0;

    double PDF_Term = 1 / (2 * PI);
    return PDF_Term;
}

Vector3 Material::RadianceColorful(Vector3 radiance)
{
    double radiusIntencity = radiance.Magnitude();
           radiance.x      = radiance.x / radiusIntencity;
           radiance.y      = radiance.y / radiusIntencity;
           radiance.z      = radiance.z / radiusIntencity;
           radiance.x      = radiance.x * albedo.r;
           radiance.y      = radiance.y * albedo.g;
           radiance.z      = radiance.z * albedo.b;
           radiance        = radiance   * radiusIntencity;

    return radiance;
}




Vector3 Material_BlinnPhong::Shade(Ray& ray_In, HitRecord& hitRecord)
{
    Vector3 result = Vector3(0, 0, 0);
    for (auto light : g_Scene->lights)
    {
        float dot = 0;
        dot =Vector3::Dot(hitRecord.normal,light->position - hitRecord.hitPoint);
        dot = dot>0?dot:0;

        result += Vector3(albedo.r * dot, albedo.g * dot, albedo.b * dot);
    }
    
    ray_In.origin = hitRecord.hitPoint + hitRecord.normal * 0.0000000001;
    ray_In.direction = Vector3::Reflect(ray_In.direction, hitRecord.normal);
    ray_In.t = 0;
    ray_In.depth++;
    Vector3 bounceColor = ray_In.Trace(Interval());
    result = result + bounceColor * std::pow(0.5, ray_In.depth);
    
    return result;

    // if(this->isEmissive) return emissiveIntensity * emissiveDistribution;
    
    // Vector3 light = Vector3(0,0,0);
    // ray_In.origin = hitRecord.hitPoint + hitRecord.normal * 0.00001;
    // //ray_In.direction = Vector3::Reflect(ray_In.direction, (hitRecord.normal+Vector3::Random(-kd,kd)).Normalized());
    // ray_In.direction = Vector3::Reflect(ray_In.direction, hitRecord.normal);
    // ray_In.t = 0;
    // ray_In.depth++;
    // light = ray_In.Trace(Interval());

    // Vector3 result = Vector3(light.x * albedo.r, light.y * albedo.g, light.z * albedo.b);
    // return result;
}




Vector3 Material_PBM::Shade(Ray& ray_In, HitRecord& hitRecord)
{
    //Ray inCopy = Ray(ray_In);
    Vector3 x = ray_In.at(ray_In.t);
    Vector3 wo = Vector3(0,0,0) - ray_In.direction;
    Vector3 normal = hitRecord.normal;
    double r = ray_In.t/100;

    Vector3 result = Vector3(0, 0, 0);

    //找到光源则不继续Trace
    if(isEmissive) 
        result = EmissiveTerm(x, wo);
    else 
        result = ReflectionTerm(ray_In, normal);

    //不知道为什么result为0时/r^2会变成无穷大，所以先这么将就一下
    if(result.Magnitude() == 0) return Vector3(0,0,0);

    result = result;//光照衰减
    return result;
}

Vector3 Material_PBM::EmissiveTerm(Vector3 wo , Vector3 point)
{
    #ifdef DEBUG_TRACERAY
    if(Random::consoleOutPutEnable)
        std::cout <<"\033[34m"<<"【【【!采样光源!】】】"<<"\033[0m";
    #endif
    return emissiveDistribution * emissiveIntensity;
}

Vector3 Material_PBM::ReflectionTerm(Ray& ray,const Vector3& normal)
{   
    Vector3 wo = Vector3(0,0,0) - ray.direction;
    Vector3 x = ray.at(ray.t);

    Vector3 Li_Term = Vector3(0, 0, 0);
    Vector3 result = Vector3(0, 0, 0);

    double PDF_Term = PDF::SampleHemisphere(x,ray,normal);
    Vector3 wi = ray.direction;

    double BRDF_Term = BRDF(x, wo, wi,normal);
    Li_Term = ray.Trace(Interval());
    double  Cos_Term = std::max(0.0, Vector3::Dot(wi, normal));
    result = BRDF_Term * Li_Term * Cos_Term / PDF_Term;

    if(result.Magnitude() == 0) return Vector3(0,0,0);
    double radiusIntencity = result.Magnitude();
    result.x = result.x / radiusIntencity;
    result.y = result.y / radiusIntencity;
    result.z = result.z / radiusIntencity;
    result.x = result.x * albedo.r;
    result.y = result.y * albedo.g;
    result.z = result.z * albedo.b;
    result   = result * radiusIntencity;
    
    return result;
}

double Material_PBM::BRDF(Vector3 x, Vector3 wo, Vector3 wi,Vector3 normal)
{
    /*
        Cook-Torrance BRDF 方程
        D: 法线分布函数
        F: 菲涅尔方程
        G: 几何遮蔽函数
        denom: 辐射分母 = 4 * Dot(wo, normal) * Dot(wi, normal)

        BRDF = D * F * G / denom
    */
    
    double D = NormalDistribution_GGX(wi, wo, normal);
    double F = FresnelTerm_Schlick(wi, normal);
    double G = GeometryOcclusionTerm_Schlick(wo, wi, normal);
    double denom = 4 * Vector3::Dot(wo, normal) * Vector3::Dot(wi, normal);

    if(Vector3::Dot(wo, normal) < 0){
        return 0;
    }
    if(Vector3::Dot(wi, normal) < 0){
        return 0;
    }

    double BRDF_Term = D * F * G / denom;
    return BRDF_Term;
}
double Material_PBM::NormalDistribution_GGX(Vector3 wi, Vector3 wo,Vector3 normal)
{
    double alpha = roughness;
    Vector3 halfVector = (wi + wo).Normalized();
    double D = Vector3::Dot(halfVector, normal);

    /*  GGX 法线分布方程
        D = a^2 / pi * ((n * h)^2 * (a^2 - 1) + 1)^2
        其中：
        a: 粗糙度
        n: 法线
        h: 半向量
        D：n*h
    */
    double result = alpha * alpha / (PI * std::pow((D * D * (alpha * alpha - 1) + 1), 2));
    return result;
}

double Material_PBM::FresnelTerm_Schlick(Vector3 wi,Vector3 normal)
{
    // Schlick近似公式 f = f0 + (1-f0)(1-cosθ)^5
    // 其中：
    // f0:  金属度 ，位于0-1之间
    // θ: 入射光线与法线夹角

    return metallic + (1 - metallic) * std::pow(1 - Vector3::Dot(wi, normal), 5);
}

double Material_PBM::GeometryOcclusionTerm_Schlick(Vector3 wo,Vector3 wi,Vector3 normal)
{
    //Schlick近似公式: g = g1(θi) * g1(θo)
    
    double cosi = Vector3::Dot(wi, normal);
    double coso = Vector3::Dot(wo, normal);
    double gi = 2/(1 + std::sqrt(1 + std::pow(roughness,2) * std::pow(cosi,2)));
    double go = 2/(1 + std::sqrt(1 + std::pow(roughness,2) * std::pow(coso,2)));

    return gi * go;
}