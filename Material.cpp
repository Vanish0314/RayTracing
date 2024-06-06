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

Vector3 Material::RadianceColorful(Vector3 radiance)
{
    double radiusIntencity = radiance.Magnitude();
        radiance.x = radiance.x / radiusIntencity;
        radiance.y = radiance.y / radiusIntencity;
        radiance.z = radiance.z / radiusIntencity;
        radiance.x = radiance.x * albedo.r;
        radiance.y = radiance.y * albedo.g;
        radiance.z = radiance.z * albedo.b;
        radiance   = radiance * radiusIntencity;

    return radiance;
}




Vector3 Material_BlinnPhong::Shade(Ray& ray_In, HitRecord& hitRecord)
{

    Vector3 I_Ambient = Vector3(0, 0, 0);
    Vector3 I_Diffuse = Vector3(0, 0, 0);
    Vector3 I_Specular = Vector3(0, 0, 0);

    I
}

Vector3 Material_BlinnPhong::EmissiveTerm(Vector3 point, Vector3 direction)
{
    //TODO: 实现 Material_Lambert 类中的 EmissiveTerm 函数
    return Vector3(0, 0, 0);
}
Vector3 Material_BlinnPhong::ReflectionTerm(Ray& ray_In, HitRecord& hitRecord)
{
    //TODO: 实现 Material_Lambert 类中的 ReflectionTerm 函数
    return Vector3(0, 0, 0);
}




Vector3 Material_PBM::Shade(Ray& ray_In, HitRecord& hitRecord)
{

    Vector3 x = ray_In.at(ray_In.t);
    Vector3 wo = Vector3(0, 0, 0) - ray_In.direction;
    int rayDepth = ray_In.depth -1;
    double r = ray_In.t;

    wo = EmissiveTerm(x, wo) + ReflectionTerm(ray_In, hitRecord);
    wo = wo / (4.0 * PI * r * r);//光照衰减
    return wo;
}

Vector3 Material_PBM::EmissiveTerm(Vector3 wo , Vector3 point)
{
    //if(isEmissive) std::cout<<"光源被采样一次"<<std::endl;
    return emissiveDistribution * emissiveIntensity;
}

Vector3 Material_PBM::ReflectionTerm(Ray& ray_In, HitRecord& hitRecord)
{
    Vector3 x = ray_In.at(ray_In.t);
    Vector3 wo = Vector3(0, 0, 0) - ray_In.direction;
    int rayDepth = ray_In.depth - 1;
    int sampleCount = PDF_SAMPLE_COUNT;
    Vector3 normal = hitRecord.normal;

    if (this->isEmissive) return Vector3(0, 0, 0);// 发光材质不计算反射
    if (ray_In.depth == 0) return Vector3(0, 0, 0); // 光线递归深度为0，不计算反射

    std::vector<Ray*> rays;
    double PDF_Term = PDF::SampleHemisphere(x, rays, sampleCount, rayDepth,normal);

    Vector3 result = Vector3(0, 0, 0);
    for (int i = 0; i < rays.size(); i++)
    {
        Vector3 wi = rays[i]->direction;

        double BRDF_Term = BRDF(x, wo, wi,normal);
        Vector3 Li_Term = rays[i]->Trace(Interval(0.00001f,__FLT_MAX__));
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

double Material_PBM::BRDF(Vector3 x, Vector3 wo, Vector3 wi,Vector3 normal)
{
    double D = NormalDistribution_GGX(wi, wo, normal);
    double F = FresnelTerm_Schlick(wi, normal);
    double G = GeometryOcclusionTerm_Schlick(wo, wi, normal);
    double denom = 4 * Vector3::Dot(wo, normal) * Vector3::Dot(wi, normal);

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
