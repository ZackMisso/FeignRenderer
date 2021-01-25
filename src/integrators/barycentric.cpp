/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

 // this is a simple integrator which visualizes the barycentric
 // coordinates during embree's intersection tests. this is used
 // as a test to see if I can use barycentric coordinates to
 // apply the wireframe material.

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

FEIGN_BEGIN()

BarycentricIntegrator::BarycentricIntegrator(FilterNode* filter,
                                             Integrator::Params* params)
    : Integrator(filter, params)
{
}

void BarycentricIntegrator::preProcess(const Scene* scene, Sampler* sampler)
{
    Integrator::preProcess(scene, sampler);
}

Color3f BarycentricIntegrator::Li(const Scene* scene,
                                  Sampler* sampler,
                                  const Ray3f& ray,
                                  bool debug) const
{
    Intersection its;

    if (!scene->intersect_non_null(ray, its))
    {
        return Color3f(-2.f);
    }

    Point3f bary = its.bary;
    float min = bary[0];
    if (bary[1] < min) min = bary[1];
    if (bary[2] < min) min = bary[2];

    if (min < 0.01)
        return Color3f(1.f);
    return Color3f(0.f);
}

FEIGN_END()
