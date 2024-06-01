#include "Material.h"

double PDF::SampleHemisphere(Vector3 point,std::vector<Ray*>& result,int sampleCount,int rayDepth)
{
    // TODO: Implement this function
    return 0.0;
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

        Vector3 BRDF_Term = BRDF(x, wo, wi);
        Vector3 Li_Term = ray->Trace(Interval(0.00001f,__FLT_MAX__));
        double  Cos_Term = std::max(0.0, Vector3::Dot(wi, normal));

        result += BRDF_Term * Li_Term * Cos_Term / PDF_Term;
    }

    result = result / sampleCount;
    return result;
}

Vector3 Material::BRDF(Vector3 x, Vector3 wo, Vector3 wi)
{
    // TODO: Implement this function
    return Vector3(0, 0, 0);
}