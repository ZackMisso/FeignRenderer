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

WhittedIntegrator::WhittedIntegrator(FilterNode *filter, Integrator::Params *params)
    : Integrator(filter, params)
{
}

void WhittedIntegrator::preProcess(const Scene *scene, Sampler *sampler)
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif

    Integrator::preProcess(scene, sampler);

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_PREPROCESS);
#endif
}

// TODO: make this not recursive
// TODO: create a volumetric version of a direct integrator
Color3f WhittedIntegrator::Li(const Scene *scene,
                              Sampler *sampler,
                              const Ray3f &cam_ray,
                              bool debug) const
{
#if CLOCKING
    Clocker::startClock(ClockerType::INTEGRATOR_INTERSECT);
#endif

    Intersection its;
    Ray3f ray = cam_ray;

    Vector3f dir = ray.dir;
    Float rr_cont_probability = (Float)0.95f;

    if (!scene->intersect_non_null(ray, its))
    {
        // LOG("returning green");
        // return Color3f(0.f, 1.f, 0.f);
#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
#endif
        return scene->env_emission(ray);
    }

#if CLOCKING
    Clocker::endClock(ClockerType::INTEGRATOR_INTERSECT);
    Clocker::startClock(ClockerType::SHADER);
    Clocker::startClock(ClockerType::SHADER_SURFACE);
#endif

    const MaterialShader *shader = scene->getShapeMaterialShader(its);

    // create the material closure
    MaterialClosure closure = MaterialClosure(sampler,
                                              &its,
                                              &ray,
                                              scene,
                                              false,
                                              true);

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
#endif

    if (closure.is_specular)
    {
#if CLOCKING
    Clocker::startClock(ClockerType::SHADER_SURFACE_SAMPLE);
#endif

        // sample the next path
        closure.wi = its.toLocal(-ray.dir);
        shader->sample(closure);

#if CLOCKING
        Clocker::endClock(ClockerType::SHADER_SURFACE_SAMPLE);
        Clocker::endClock(ClockerType::SHADER_SURFACE);
        Clocker::endClock(ClockerType::SHADER);
        Clocker::startClock(ClockerType::INTEGRATOR_RR);
#endif

        // random termination
        // TODO: make this happen before the above closure
        if (sampler->next1D() > rr_cont_probability) {
#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_RR);
#endif

            return Color3f(0.f);
        }

#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_RR);
        Clocker::startClock(ClockerType::INTEGRATOR_EVAL);
#endif

        Ray3f new_ray(its.p,
                      its.toWorld(closure.wo),
                      Epsilon,
                      std::numeric_limits<Float>::infinity(),
                      ray.depth + 1);

        // if contribution is zero terminate the path to save computation
        Color3f beta = closure.albedo * (ONE / rr_cont_probability);

#if CLOCKING
        Clocker::endClock(ClockerType::INTEGRATOR_EVAL);
#endif

        if (beta.isZero()) {
            return closure.nee + closure.emission;
        }

        // TODO: maybe turn this into an iterative version of whitted
        Color3f recur = Li(scene, sampler, new_ray);

        return beta * recur +
               closure.nee + closure.emission;
    }

#if CLOCKING
    Clocker::endClock(ClockerType::SHADER_SURFACE);
    Clocker::endClock(ClockerType::SHADER);
#endif

    return closure.emission + closure.nee + closure.emission;
}

FEIGN_END()
