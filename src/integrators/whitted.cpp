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

WhittedIntegrator::WhittedIntegrator(FilterNode* filter, Integrator::Params* params)
    : Integrator(filter, params)
{
}

void WhittedIntegrator::preProcess(const Scene* scene, Sampler* sampler)
{
    Integrator::preProcess(scene, sampler);
}

// TODO: create a volumetric version of a direct integrator
Color3f WhittedIntegrator::Li(const Scene* scene,
                              Sampler* sampler,
                              const Ray3f& cam_ray,
                              bool debug) const
{
    Intersection its;
    Ray3f ray = cam_ray;

    Vector3f dir = ray.dir;
    Float rr_cont_probability = 0.95f;

    if (!scene->intersect_non_null(ray, its))
    {
        return scene->env_emission(ray);
    }

    const MaterialShader* shader = scene->getShapeMaterialShader(its);

    // create the material closure
    MaterialClosure closure = MaterialClosure(sampler,
                                              &its,
                                              &ray,
                                              scene,
                                              false,
                                              true);

    // evaluate the material shader
    shader->evaluate(closure);

    // accumulate the shadow rays
    closure.accumulate_shadow_rays(shader);

    if (closure.is_specular)
    {
        // sample the next path
        closure.wi = its.toLocal(-ray.dir);
        shader->sample(closure);

        // random termination
        if (sampler->next1D() > rr_cont_probability) return Color3f(0.f);

        Ray3f new_ray(its.p,
                      its.toWorld(closure.wo),
                      Epsilon,
                      std::numeric_limits<Float>::infinity(),
                      ray.depth + 1);

        Color3f beta = closure.albedo * (1.f / rr_cont_probability);
        if (beta.isZero()) return closure.nee + closure.emission;

        Color3f recur = Li(scene, sampler, new_ray);

        return beta * recur +
               closure.nee + closure.emission;
    }

    return closure.emission + closure.nee + closure.emission;
}

FEIGN_END()
