/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

FEIGN_BEGIN()

NiceNormalIntegrator::NiceNormalIntegrator(FilterNode *filter,
                                           Integrator::Params *params)
    : Integrator(filter, params)
{
}

void NiceNormalIntegrator::preProcess(const Scene *scene, Sampler *sampler)
{
    Integrator::preProcess(scene, sampler);
}

Color3f NiceNormalIntegrator::Li(const Scene *scene,
                                 Sampler *sampler,
                                 const Ray3f &ray,
                                 bool debug) const
{
    Intersection its;

    if (!scene->intersect_non_null(ray, its))
    {
        return Color3f(0.f);
    }

    Normal3f shad_n = (its.s_frame.n + Vector3f(1.f, 1.f, 1.f)) * 0.5;

    return Color3f(shad_n(0), shad_n(1), shad_n(2));
}

FEIGN_END()
