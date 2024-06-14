/*
 * @Author: Vanish
 * @Date: 2024-06-02 04:28:38
 * @LastEditTime: 2024-06-14 23:20:52
 * Also View: http://vanishing.cc
 * Copyright@ https://creativecommons.org/licenses/by/4.0/deed.zh-hans
 */
#include "Material.h"


double PDF::SampleHemisphere(Vector3 point,Ray& result,Vector3 normal)
{

    Vector3 randomVec = Vector3::RandomInHemisphere(normal);
    result.origin = point + normal * 0.0000001;
    result.direction = randomVec.Normalized();
    result.depth++;

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
    /*
    Vector3 I_Ambient = Vector3(0, 0, 0);
    Vector3 I_Diffuse = Vector3(0, 0, 0);
    Vector3 I_Specular = Vector3(0, 0, 0);
    */

    if (ray_In.depth <= 0)
        return Vector3(0,0,0);
    
    Ray scattered =Ray(hitRecord.hitPoint + hitRecord.normal * 0.0000000001,Vector3::Reflect(ray_In.direction, hitRecord.normal));
    scattered.depth = ray_In.depth - 1;
    Vector3 attenuation = Vector3(albedo.r, albedo.g, albedo.b);

    return attenuation + (0.5 * scattered.Trace(Interval()));

}




Vector3 Material_PBM::Shade(Ray& ray_In, HitRecord& hitRecord)
{

    Vector3 x = ray_In.at(ray_In.t);
    Vector3 wo = Vector3(0,0,0) - ray_In.direction;
    Vector3 normal = hitRecord.normal;
    double r = ray_In.t;

    //找到光源则不继续Trace
    if(isEmissive) 
        wo = EmissiveTerm(x, wo);
    else 
        wo = ReflectionTerm(ray_In, normal);

    wo = wo / (4.0 * PI * r * r);//光照衰减
    return wo;
}

Vector3 Material_PBM::EmissiveTerm(Vector3 wo , Vector3 point)
{
    #ifdef DEBUG_TRACERAY
            std::cout <<"是光源"<< std::endl;
    #endif
    return emissiveDistribution * emissiveIntensity;
}

Vector3 Material_PBM::ReflectionTerm(Ray& ray,const Vector3& normal)
{   
    Vector3 wo = Vector3(0,0,0) - ray.direction;
    Vector3 x = ray.at(ray.t);

    double PDF_Term = PDF::SampleHemisphere(x,ray,normal);
    Vector3 wi = ray.direction;

    Vector3 result = Vector3(0, 0, 0);

    double BRDF_Term = BRDF(x, wo, wi,normal);
    Vector3 Li_Term = ray.Trace(Interval(0.00001f,__FLT_MAX__));
    double  Cos_Term = std::max(0.0, Vector3::Dot(wi, normal));
    result = BRDF_Term * Li_Term * Cos_Term / PDF_Term;

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