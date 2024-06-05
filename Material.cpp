#include "Material.h"

/// @brief 采样半球面，返回一系列光线
/// @param point 采样点
/// @param result 结果容器
/// @param sampleCount 需要的光线数量
/// @param rayDepth 光线递归深度
/// @return PDF 项的值
double PDF::SampleHemisphere(Vector3 point,std::vector<Ray*>& result,int sampleCount,int rayDepth,Vector3 normal)
{
    for (int i = 0; i < sampleCount; i++)
    {
        Vector3 randomVec = Vector3::RandomInHemisphere(normal);
        Ray* ray = new Ray(point + normal * 0.0000001, randomVec.Normalized());
        ray->depth = rayDepth -1;
        result.push_back(ray);
    }
    
    double PDF_Term = 1 / (2 * PI);
    return PDF_Term;
}

Vector3 Material::Shade_Lambert(const Ray& ray,double u,double v,Vector3 normal,Vector3 point)
{
    if(ray.depth == 0) 
    {
        for(auto light : g_Scene->lights)
        {
            double t = (ray.origin - light->position).Magnitude();
            Vector3 l = light->material.get()->EmissiveTerm(Vector3(0,0,0),Vector3(0,0,0));
            l = l/(4.0*PI*t*t);
        }
    }

    Vector3 result = Vector3(0, 0, 0);

    /*
    for(auto light : g_Scene->lights)
    {
        double cos = Vector3::Dot(normal, (light->position - point).Normalized());
        double kd = metallic;
        Vector3 Il = light->material.get()->emissiveDistribution * light->material.get()->emissiveIntensity;

        Vector3 I = kd * Il * std::max(cos, 0.0);

        result += I;
    }

    result = Vector3(
        result.x * albedo.r,
        result.y * albedo.g,
        result.z * albedo.b
    );
    */

    if(isEmissive)
    {
        result = EmissiveTerm(point, Vector3(0,0,0)-ray.direction);
        double r = ray.t;
        result = result / (4.0 * PI * r * r);
        return result;
    }
    else
    {
        Vector3 from = ray.at(ray.t) + normal * 0.000001;
        Vector3 to = Vector3::Reflect(ray.direction, normal);
        Ray* newRay = new Ray(from, to);
        newRay->depth = ray.depth - 1;

        Vector3 result = newRay->Trace_Lambert(Interval(0.00001f, __FLT_MAX__));
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
}


Vector3 Material::Shade_PBS(const Ray& ray,int sampleCount,double u,double v,Vector3 normal)
{
    Vector3 x = ray.at(ray.t);
    Vector3 wo = Vector3(0, 0, 0) - ray.direction;
    int rayDepth = ray.depth -1;
    double r = ray.t;

    wo = EmissiveTerm(x, wo) + ReflectionTerm(x, wo,normal, sampleCount, rayDepth);
    wo = wo / (4.0 * PI * r * r);//光照衰减
    return wo;
}

Vector3 Material::EmissiveTerm(Vector3 x, Vector3 wo)
{
    //if(isEmissive) std::cout<<"光源被采样一次"<<std::endl;
    return emissiveDistribution * emissiveIntensity;
}

Vector3 Material::ReflectionTerm(Vector3 x, Vector3 wo,Vector3 normal, int sampleCount,int rayDepth)
{
    if (this->isEmissive) return Vector3(0, 0, 0);// 发光材质不计算反射
    if (rayDepth == 0) return Vector3(0, 0, 0); // 光线递归深度为0，不计算反射

    std::vector<Ray*> rays;
    double PDF_Term = PDF::SampleHemisphere(x, rays, sampleCount, rayDepth,normal);

    Vector3 result = Vector3(0, 0, 0);
    for (int i = 0; i < rays.size(); i++)
    {
        Vector3 wi = rays[i]->direction;

        double BRDF_Term = BRDF(x, wo, wi,normal);
        Vector3 Li_Term = rays[i]->Trace_PBR(Interval(0.00001f,__FLT_MAX__));
        double  Cos_Term = std::max(0.0, Vector3::Dot(wi, normal));

        result += BRDF_Term * Li_Term * Cos_Term / PDF_Term;
    }

    result = result / sampleCount;
    double radiusIntencity = result.Magnitude();
    result.x = result.x / radiusIntencity;
    result.y = result.y / radiusIntencity;
    result.z = result.z / radiusIntencity;
    result.x = result.x * albedo.r;
    result.y = result.y * albedo.g;
    result.z = result.z * albedo.b;
    result   = result * radiusIntencity;
    //释放内存
    for(int i = rays.size()-1; i >= 0; i--)
    {
        delete rays[i];
    }
    rays.clear();
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
