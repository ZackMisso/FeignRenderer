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
    CLOCKER_START_ONE(ClockerType::INTEGRATOR_PREPROCESS)

    Integrator::preProcess(scene, sampler);

    CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_PREPROCESS)
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

        CLOCKER_START_ONE(ClockerType::INTEGRATOR_INTERSECT)

        Intersection its;

        if (!scene->intersect_non_null(ray, its))
        {
            CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_INTERSECT)
            
            Li += beta * scene->env_emission(ray);
            break;
        }

        CLOCKER_START_TWO_STOP_ONE(ClockerType::SHADER,
                                   ClockerType::SHADER_SURFACE,
                                   ClockerType::INTEGRATOR_INTERSECT)

        const MaterialShader *shader = scene->getShapeMaterialShader(its);

        closure.its = &its;
        closure.ray = &ray;
        closure.wi = its.toLocal(-ray.dir);
        closure.emission = COLOR_BLACK;
        closure.nee = COLOR_BLACK;
        closure.albedo = COLOR_BLACK;

        CLOCKER_START_ONE(ClockerType::SHADER_SURFACE_EVAL)

        // evaluate the material shader
        shader->evaluate(closure);

        CLOCKER_START_STOP_ONE(ClockerType::INTEGRATOR_NEE,
                               ClockerType::SHADER_SURFACE_EVAL)

        // accumulate the shadow rays
        closure.accumulate_shadow_rays(shader);

        CLOCKER_START_ONE_STOP_THREE(ClockerType::INTEGRATOR_RR,
                                     ClockerType::INTEGRATOR_NEE,
                                     ClockerType::SHADER_SURFACE,
                                     ClockerType::SHADER)

        Float rr_prob = std::min(beta.maxValue(), ONE);

        // random termination
        if (sampler->next1D() > rr_prob)
        {
            Li += beta * (closure.emission + closure.nee);

            CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_RR)

            break;
        }

        CLOCKER_START_THREE_STOP_ONE(ClockerType::SHADER,
                                     ClockerType::SHADER_SURFACE,
                                     ClockerType::SHADER_SURFACE_SAMPLE,
                                     ClockerType::INTEGRATOR_RR)

        // sample the next path
        closure.wi = its.toLocal(-ray.dir);
        shader->sample(closure);

        if (closure.pdf == ZERO)
        {
            Li += beta * (closure.emission + closure.nee);

            CLOCKER_STOP_THREE(ClockerType::SHADER_SURFACE_SAMPLE,
                               ClockerType::SHADER_SURFACE,
                               ClockerType::SHADER)

            break;
        }

        CLOCKER_START_ONE_STOP_THREE(ClockerType::INTEGRATOR_EVAL,
                                     ClockerType::SHADER_SURFACE_SAMPLE,
                                     ClockerType::SHADER_SURFACE,
                                     ClockerType::SHADER)

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

        CLOCKER_STOP_ONE(ClockerType::INTEGRATOR_EVAL)
    }

    return Li;
}

FEIGN_END()
