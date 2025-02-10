/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/math/warp.h>
#include <feign/stats/clocker.h>

FEIGN_BEGIN()

Ambient_Occlusion_Integrator::Ambient_Occlusion_Integrator(FilterNode *filter,
                                                           Integrator::Params *params)
    : Integrator(filter, params)
{
    // does nothing
}

void Ambient_Occlusion_Integrator::preProcess(const Scene *scene,
                                              Sampler *sampler)
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif

    Integrator::preProcess(scene, sampler);

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif
}

Color3f Ambient_Occlusion_Integrator::Li(const Scene *scene,
                                         Sampler *sampler,
                                         const Ray3f &ray,
                                         bool debug) const
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_INTERSECT);
#endif

    Intersection its;

    Vector3f dir = ray.dir;

    if (!scene->intersect_non_null(ray, its))
    {
#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
#endif

        return Color3f(0.f);
    }

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
    // this could also be considered integrator_intersect, but since the
    // meat of an amb occlusion integrator is evaluating a shadow ray,
    // I am demarcing this as integrator_eval. Note: maybe do both?
    Clocker::startClock(ClockerType::INTEGRATOR_EVAL);
#endif

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
#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_EVAL);
#endif

        return Color3f(1.f);
    }

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_EVAL);
#endif

    return Color3f(0.0);
}

FEIGN_END()
