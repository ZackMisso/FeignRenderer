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

NormalIntegrator::NormalIntegrator(FilterNode *filter,
                                   Integrator::Params *params)
    : Integrator(filter, params)
{
}

void NormalIntegrator::preProcess(const Scene *scene, Sampler *sampler)
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif

    Integrator::preProcess(scene, sampler);

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif
}

Color3f NormalIntegrator::Li(const Scene *scene,
                             Sampler *sampler,
                             const Ray3f &ray,
                             bool debug) const
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_INTERSECT);
#endif

    Intersection its;

    if (!scene->intersect_non_null(ray, its))
    {
#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
#endif

        return Color3f(0.f);
    }

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
    Clocker::startClock(ClockerType::INTEGRATOR_EVAL);
#endif

    Normal3f shad_n = ~(its.s_frame.n);

    if (shad_n(0) < 0.f || shad_n(1) < 0.f || shad_n(2) < 0.f)
    {
        return Color3f(0.f);
    }

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_EVAL);
#endif

    return Color3f(shad_n(0), shad_n(1), shad_n(2));
}

FEIGN_END()
