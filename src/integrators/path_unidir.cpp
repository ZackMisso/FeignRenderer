/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

FEIGN_BEGIN()

Path_Unidirectional_Integrator::Path_Unidirectional_Integrator(FilterNode *filter,
                                                               Integrator::Params *params)
    : Integrator(filter, params) {}

void Path_Unidirectional_Integrator::preProcess(const Scene *scene, Sampler *sampler)
{
    Integrator::preProcess(scene, sampler);
}

Color3f Path_Unidirectional_Integrator::Li(const Scene *scene,
                                           Sampler *sampler,
                                           const Ray3f &cam_ray,
                                           bool debug) const
{
    Color3f Li = Color3f(0.f);
    Color3f beta = Color3f(1.f);
    Ray3f ray = cam_ray;

    // predefine this so it does not have to get recreated every loop
    MaterialClosure closure = MaterialClosure(sampler,
                                              scene,
                                              false,
                                              true);

    for (int bounces = 0; bounces < max_bounces; ++bounces)
    {
        // LOG("max_bounces: " + std::to_string(max_bounces));
        // LOG("bounces: " + std::to_string(bounces));
        if (beta.isZero())
            break;

        Intersection its;

        if (!scene->intersect_non_null(ray, its))
        {
            Li += beta * scene->env_emission(ray);
            break;
        }

        const MaterialShader *shader = scene->getShapeMaterialShader(its);

        closure.its = &its;
        closure.ray = &ray;
        closure.wi = its.toLocal(-ray.dir);
        closure.emission = COLOR_BLACK;
        closure.nee = COLOR_BLACK;
        closure.albedo = COLOR_BLACK;

        // evaluate the material shader
        shader->evaluate(closure);

        // accumulate the shadow rays
        closure.accumulate_shadow_rays(shader);

        Float rr_prob = std::min(beta.maxValue(), 1.f);

        // random termination
        if (sampler->next1D() > rr_prob)
        {
            Li += beta * (closure.emission + closure.nee);
            break;
        }

        // sample the next path
        closure.wi = its.toLocal(-ray.dir);
        shader->sample(closure);

        if (closure.pdf == 0.f)
        {
            Li += beta * (closure.emission + closure.nee);
            break;
        }

        ray = Ray3f(its.p,
                    its.toWorld(closure.wo),
                    Epsilon,
                    std::numeric_limits<Float>::infinity(),
                    ray.depth + 1);

        Float cosTerm = its.s_frame.n % ray.dir;
        if (cosTerm < 0.f)
            cosTerm = -cosTerm;
        if (closure.is_specular)
            cosTerm = 1.f;

        Li += beta * (closure.nee + closure.emission);
        beta *= closure.albedo * cosTerm / (closure.pdf * rr_prob);
    }

    return Li;
}

FEIGN_END()
