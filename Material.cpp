#include "Material.h"

/// @brief 采样半球面，返回一系列光线
/// @param point 采样点
/// @param result 结果容器
/// @param sampleCount 需要的光线数量
/// @param rayDepth 光线递归深度
/// @return PDF 项的值
double PDF::SampleHemisphere(Vector3 point,std::vector<Ray*>& result,int sampleCount,int rayDepth)
{
    double pdf = 1.0 / (2 * PI);
    double phi = 2 * PI * std::rand();
    double cosTheta = 2 * std::rand() - 1;
    double sinTheta = std::sqrt(1 - cosTheta * cosTheta);

    Vector3 w = Vector3(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
    Vector3 u = Vector3::Cross(Vector3(0, 1, 0), w).Normalized();
    Vector3 v = Vector3::Cross(w, u);

    for (int i = 0; i < sampleCount; i++)
    {
        double r1 = std::rand();
        double r2 = std::rand();
        double x = std::sqrt(r1) * std::cos(2 * PI * r2);
        double y = std::sqrt(r1) * std::sin(2 * PI * r2);
        double z = std::sqrt(1 - r1);

        Vector3 direction = x * u + y * v + z * w;
        Ray* ray = new Ray(point, direction);
        ray->depth = rayDepth - 1;
        result.push_back(ray);
    }

    return pdf;
}

Vector3 Material::Shade(const Ray& ray,int sampleCount,double u,double v,Vector3 normal)
{
    Vector3 x = ray.at(ray.t);
    Vector3 wo = Vector3(0, 0, 0) - ray.direction;
    int rayDepth = ray.depth -1;
    double r = ray.t;

    wo = EmissiveTerm(x, wo) + ReflectionTerm(x, wo,normal, sampleCount, rayDepth);
    wo = wo/4 * PI * r * r;//光照衰减
    return wo;
}

Vector3 Material::EmissiveTerm(Vector3 x, Vector3 wo)
{
    return emissiveDistribution * emissiveIntensity;
}

Vector3 Material::ReflectionTerm(Vector3 x, Vector3 wo,Vector3 normal, int sampleCount,int rayDepth)
{
    if (rayDepth == 0) return Vector3(0, 0, 0);

    std::vector<Ray*> rays;
    double PDF_Term = PDF::SampleHemisphere(x, rays, sampleCount, rayDepth);

    Vector3 result = Vector3(0, 0, 0);
    for (int i = 0; i < rays.size(); i++)
    {
        Ray* ray = rays[i];

        Vector3 wi = Vector3(0,0,0)-(ray->direction);

        double BRDF_Term = BRDF(x, wo, wi,normal);
        Vector3 Li_Term = ray->Trace(Interval(0.00001f,__FLT_MAX__));
        double  Cos_Term = std::max(0.0, Vector3::Dot(wi, normal));

        result += BRDF_Term * Li_Term * Cos_Term / PDF_Term;
    }

    result = result / sampleCount;
    return result;
}

double Material::BRDF(Vector3 x, Vector3 wo, Vector3 wi,Vector3 normal)
{
    double D = NormalDistribution_GGX(wi, wo, normal);
    double F = FresnelTerm_Schlick(wi, normal);
    double G = GeometryOcclusionTerm_Schlick(wo, wi, normal);
    double denom = 4 * Vector3::Dot(wo, normal) * Vector3::Dot(wi, normal);

    double BRDF_Term = D * F * G / denom;
    return BRDF_Term;
}
double Material::NormalDistribution_GGX(Vector3 wi, Vector3 wo,Vector3 normal)
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

double Material::FresnelTerm_Schlick(Vector3 wi,Vector3 normal)
{
    // Schlick近似公式 f = f0 + (1-f0)(1-cosθ)^5
    // 其中：
    // f0:  金属度 ，位于0-1之间
    // θ: 入射光线与法线夹角

    return metallic + (1 - metallic) * std::pow(1 - Vector3::Dot(wi, normal), 5);
}

double Material::GeometryOcclusionTerm_Schlick(Vector3 wo,Vector3 wi,Vector3 normal)
{
    //Schlick近似公式: g = g1(θi) * g1(θo)
    
    double cosi = Vector3::Dot(wi, normal);
    double coso = Vector3::Dot(wo, normal);
    double gi = 2/(1 + std::sqrt(1 + std::pow(roughness,2) * std::pow(cosi,2)));
    double go = 2/(1 + std::sqrt(1 + std::pow(roughness,2) * std::pow(coso,2)));

    return gi * go;
}
