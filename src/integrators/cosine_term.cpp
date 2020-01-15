/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

CosineTermIntegrator::CosineTermIntegrator(FilterNode* filter,
                                           std::string location,
                                           long max_time,
                                           long max_heuristic)
    : Integrator(filter, location, max_time, max_heuristic)
{
}

void CosineTermIntegrator::preProcess()
{
    Integrator::preProcess();
}

Color3f CosineTermIntegrator::Li(const Scene* scene,
                             Sampler* sampler,
                             const Ray3f& ray) const
{
    Intersection its;

    if (!scene->intersect(ray, its))
    {
        return Color3f(-2.f);
    }

    Normal3f shad_n = its.s_frame.n;
    Vector3f vect_n = (Vector3f)shad_n;
    Float cosine_term = vect_n % ray.dir;

    return Color3f(cosine_term, cosine_term, cosine_term);
}
