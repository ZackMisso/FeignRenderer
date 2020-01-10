/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

NormalIntegrator::NormalIntegrator(FilterNode* filter,
                                   std::string location,
                                   long max_time,
                                   long max_heuristic)
    : Integrator(filter, location, max_time, max_heuristic)
{
}

void NormalIntegrator::preProcess()
{
    Integrator::preProcess();
}

Color3f NormalIntegrator::Li(const Scene* scene,
                             Sampler* sampler,
                             const Ray3f& ray) const
{
    Intersection its;

    if (!scene->intersect(ray, its))
    {
        return Color3f(0.f);
    }

    Normal3f shad_n = ~(its.s_frame.n);

    if (shad_n(0) < 0.f || shad_n(1) < 0.f || shad_n(2) < 0.f)
    {
        return Color3f(0.f);
    }

    return Color3f(shad_n(0), shad_n(1), shad_n(2));
}
