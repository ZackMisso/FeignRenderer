/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/stats/clocker.h>

FEIGN_BEGIN()

NiceNormalIntegrator::NiceNormalIntegrator(FilterNode *filter,
                                           Integrator::Params *params)
    : Integrator(filter, params)
{
}

void NiceNormalIntegrator::preProcess(const Scene *scene, Sampler *sampler)
{
    CLOCKER_START_ONE(ClockerType::INTEGRATOR_PREPROCESS)

    Integrator::preProcess(scene, sampler);

    CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_PREPROCESS)
}

Color3f NiceNormalIntegrator::Li(const Scene *scene,
                                 Sampler *sampler,
                                 const Ray3f &ray,
                                 bool debug) const
{
    CLOCKER_START_ONE(ClockerType::INTEGRATOR_INTERSECT)

    Intersection its;

    if (!scene->intersect_non_null(ray, its))
    {
        CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_INTERSECT)

        return Color3f(0.f);
    }

    CLOCKER_START_STOP_ONE(ClockerType::INTEGRATOR_EVAL,
                           ClockerType::INTEGRATOR_INTERSECT)

    Normal3f shad_n = (its.s_frame.n + Vector3f(1.f, 1.f, 1.f)) * 0.5;

    CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_EVAL)

    return Color3f(shad_n(0), shad_n(1), shad_n(2));
}

FEIGN_END()
