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

Path_Unidirectional_Integrator::Path_Unidirectional_Integrator(FilterNode *filter,
                                                               Integrator::Params *params)
    : Integrator(filter, params) {}

void Path_Unidirectional_Integrator::preProcess(const Scene *scene, Sampler *sampler)
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif

    Integrator::preProcess(scene, sampler);

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif
}

Color3f Path_Unidirectional_Integrator::Li(const Scene *scene,
                                           Sampler *sampler,
                                           const Ray3f &cam_ray,
                                           bool debug) const
{
    Color3f Li = Color3f(ZERO);
    Color3f beta = Color3f(ONE);
    Ray3f ray = cam_ray;

    // predefine this so it does not have to get recreated every loop
    MaterialClosure closure = MaterialClosure(sampler,
                                              scene,
                                              false,
                                              true);

    for (int bounces = 0; bounces < max_bounces; ++bounces)
    {
        if (beta.isZero())
            break;

#if CLOCKING
        Clocker::startClock(ClockerType::INTEGRATOR_INTERSECT);
#endif

        Intersection its;

        if (!scene->intersect_non_null(ray, its))
        {
#if CLOCKING
            Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
#endif
            Li += beta * scene->env_emission(ray);
            break;
        }

#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
        Clocker::startClock(ClockerType::SHADER);
        Clocker::startClock(ClockerType::SHADER_SURFACE);
#endif

        const MaterialShader *shader = scene->getShapeMaterialShader(its);

        closure.its = &its;
        closure.ray = &ray;
        closure.wi = its.toLocal(-ray.dir);
        closure.emission = COLOR_BLACK;
        closure.nee = COLOR_BLACK;
        closure.albedo = COLOR_BLACK;

#if CLOCKING
        Clocker::startClock(ClockerType::SHADER_SURFACE_EVAL);
#endif

        // evaluate the material shader
        shader->evaluate(closure);

#if CLOCKING
        Clocker::endClock(ClockerType::SHADER_SURFACE_EVAL);
        Clocker::startClock(ClockerType::INTEGRATOR_NEE);
#endif

        // accumulate the shadow rays
        closure.accumulate_shadow_rays(shader);

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_NEE);
    Clocker::endClock(ClockerType::SHADER_SURFACE);
    Clocker::endClock(ClockerType::SHADER);
    Clocker::startClock(ClockerType::INTEGRATOR_RR);
#endif

        Float rr_prob = std::min(beta.maxValue(), ONE);

        // random termination
        if (sampler->next1D() > rr_prob)
        {
            Li += beta * (closure.emission + closure.nee);

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_RR);
#endif

            break;
        }

#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_RR);
        Clocker::startClock(ClockerType::SHADER);
        Clocker::startClock(ClockerType::SHADER_SURFACE);
        Clocker::startClock(ClockerType::SHADER_SURFACE_SAMPLE);
#endif

        // sample the next path
        closure.wi = its.toLocal(-ray.dir);
        shader->sample(closure);

        if (closure.pdf == ZERO)
        {
            Li += beta * (closure.emission + closure.nee);

#if CLOCKING
        Clocker::endClock(ClockerType::SHADER_SURFACE_SAMPLE);
        Clocker::endClock(ClockerType::SHADER_SURFACE);
        Clocker::endClock(ClockerType::SHADER);
#endif

            break;
        }

#if CLOCKING
        Clocker::endClock(ClockerType::SHADER_SURFACE_SAMPLE);
        Clocker::endClock(ClockerType::SHADER_SURFACE);
        Clocker::endClock(ClockerType::SHADER);
        Clocker::startClock(ClockerType::INTEGRATOR_EVAL);
#endif

        ray = Ray3f(its.p,
                    its.toWorld(closure.wo),
                    Epsilon,
                    std::numeric_limits<Float>::infinity(),
                    ray.depth + 1);

        Float cosTerm = its.s_frame.n % ray.dir;
        if (cosTerm < ZERO)
            cosTerm = -cosTerm;
        if (closure.is_specular)
            cosTerm = ONE;

        Li += beta * (closure.nee + closure.emission);
        beta *= closure.albedo * cosTerm / (closure.pdf * rr_prob);

#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_EVAL);
#endif
    }

    return Li;
}

FEIGN_END()
