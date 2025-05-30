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
    CLOCKER_START_ONE(ClockerType::INTEGRATOR_PREPROCESS)

    Integrator::preProcess(scene, sampler);

    CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_PREPROCESS)
}

// TODO: make this not recursive
// TODO: create a volumetric version of a direct integrator
Color3f WhittedIntegrator::Li(const Scene *scene,
                              Sampler *sampler,
                              const Ray3f &cam_ray,
                              bool debug) const
{
    CLOCKER_START_ONE(ClockerType::INTEGRATOR_INTERSECT)

    Intersection its;
    Ray3f ray = cam_ray;

    Vector3f dir = ray.dir;
    Float rr_cont_probability = (Float)0.95f;

    if (!scene->intersect_non_null(ray, its))
    {
        CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_INTERSECT)

        return scene->env_emission(ray);
    }

    CLOCKER_START_TWO_STOP_ONE(ClockerType::SHADER,
                               ClockerType::SHADER_SURFACE,
                               ClockerType::INTEGRATOR_INTERSECT)

    const MaterialShader *shader = scene->getShapeMaterialShader(its);

    // create the material closure
    MaterialClosure closure = MaterialClosure(sampler,
                                              &its,
                                              &ray,
                                              scene,
                                              false,
                                              true);

    CLOCKER_START_ONE(ClockerType::SHADER_SURFACE_EVAL)

    // evaluate the material shader
    shader->evaluate(closure);

    CLOCKER_START_STOP_ONE(ClockerType::INTEGRATOR_NEE,
                           ClockerType::SHADER_SURFACE_EVAL)

    // accumulate the shadow rays
    closure.accumulate_shadow_rays(shader);

    CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_NEE)

    if (closure.is_specular)
    {
        CLOCKER_START_ONE(ClockerType::SHADER_SURFACE_SAMPLE)

        // sample the next path
        closure.wi = its.toLocal(-ray.dir);
        shader->sample(closure);

        CLOCKER_START_ONE_STOP_THREE(ClockerType::INTEGRATOR_RR,
                                     ClockerType::SHADER_SURFACE_SAMPLE,
                                     ClockerType::SHADER_SURFACE,
                                     ClockerType::SHADER)

        // random termination
        // TODO: make this happen before the above closure
        if (sampler->next1D() > rr_cont_probability) {
            CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_RR)

            return Color3f(0.f);
        }

        CLOCKER_START_STOP_ONE(ClockerType::INTEGRATOR_EVAL,
                               ClockerType::INTEGRATOR_RR)

        Ray3f new_ray(its.p,
                      its.toWorld(closure.wo),
                      Epsilon,
                      std::numeric_limits<Float>::infinity(),
                      ray.depth + 1);

        // if contribution is zero terminate the path to save computation
        Color3f beta = closure.albedo * (ONE / rr_cont_probability);

        CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_EVAL)

        if (beta.isZero()) {
            return closure.nee + closure.emission;
        }

        // TODO: maybe turn this into an iterative version of whitted
        Color3f recur = Li(scene, sampler, new_ray);

        return beta * recur +
               closure.nee + closure.emission;
    }

    CLOCKER_STOP_TWO(ClockerType::SHADER_SURFACE,
                     ClockerType::SHADER)

    return closure.emission + closure.nee + closure.emission;
}

FEIGN_END()
