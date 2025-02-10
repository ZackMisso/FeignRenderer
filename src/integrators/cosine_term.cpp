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

// TODO: create a debug folder for integrators which visualize specific
//       quantities, such as this one.

FEIGN_BEGIN()

CosineTermIntegrator::CosineTermIntegrator(FilterNode *filter,
                                           Integrator::Params *params)
    : Integrator(filter, params)
{
}

void CosineTermIntegrator::preProcess(const Scene *scene, Sampler *sampler)
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif

    Integrator::preProcess(scene, sampler);

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif
}

Color3f CosineTermIntegrator::Li(const Scene *scene,
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

        return Color3f(-2.f);
    }

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
    Clocker::startClock(ClockerType::INTEGRATOR_EVAL);
#endif

    Normal3f shad_n = its.s_frame.n;
    Vector3f vect_n = (Vector3f)shad_n;
    Float cosine_term = vect_n % ray.dir;

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_EVAL);
#endif

    return Color3f(cosine_term, cosine_term, cosine_term);
}

FEIGN_END()
