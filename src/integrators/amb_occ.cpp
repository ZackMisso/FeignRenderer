/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/
#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/math/warp.h>

Ambient_Occlusion_Integrator::Ambient_Occlusion_Integrator(FilterNode* filter,
                                                           Integrator::Params* params)
    : Integrator(filter, params)
{
    // does nothing
}

void Ambient_Occlusion_Integrator::preProcess()
{
    Integrator::preProcess();
}

Color3f Ambient_Occlusion_Integrator::Li(const Scene* scene,
                                         Sampler* sampler,
                                         const Ray3f& ray) const
{
    Intersection its;

    Vector3f dir = ray.dir;

    if (!scene->intersect_non_null(ray, its))
    {
        return Color3f(0.f);
    }

    Point2f point = sampler->next2D();
    Vector3f sample_dir = WarpSpace::sqrToCosHemi(point);
    Float pdf = WarpSpace::sqrToCosHemiPdf(sample_dir);

    Ray3f shadow_ray(its.p,
                     its.toWorldGeom(sample_dir),
                     Epsilon,
                     std::numeric_limits<Float>::infinity());

    Intersection shadow_its;
    if (!scene->intersect_non_null(shadow_ray, shadow_its))
    {
        return Color3f(1.f);
    }

    return Color3f(0.0);
}
