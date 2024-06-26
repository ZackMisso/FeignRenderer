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

CosineTermIntegrator::CosineTermIntegrator(FilterNode *filter,
                                           Integrator::Params *params)
    : Integrator(filter, params)
{
}

void CosineTermIntegrator::preProcess(const Scene *scene, Sampler *sampler)
{
    Integrator::preProcess(scene, sampler);
}

Color3f CosineTermIntegrator::Li(const Scene *scene,
                                 Sampler *sampler,
                                 const Ray3f &ray,
                                 bool debug) const
{
    Intersection its;

    if (!scene->intersect_non_null(ray, its))
    {
        return Color3f(-2.f);
    }

    Normal3f shad_n = its.s_frame.n;
    Vector3f vect_n = (Vector3f)shad_n;
    Float cosine_term = vect_n % ray.dir;

    return Color3f(cosine_term, cosine_term, cosine_term);
}

FEIGN_END()
