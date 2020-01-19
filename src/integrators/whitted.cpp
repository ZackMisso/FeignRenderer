/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/
#include <feign/core/integrator.h>
#include <feign/core/scene.h>

WhittedIntegrator::WhittedIntegrator(FilterNode* filter,
                                     std::string location,
                                     long max_time,
                                     long max_heuristic)
    : Integrator(filter, location, max_time, max_heuristic)
{
}

void WhittedIntegrator::preProcess()
{
    Integrator::preProcess();
}

Color3f WhittedIntegrator::Li(const Scene* scene,
                              Sampler* sampler,
                              const Ray3f& ray) const
{
    Intersection its;

    Vector3f dir = ray.dir;
    Float rr_cont_probability = 0.95f;

    if (!scene->intersect(ray, its))
    {
        return Color3f(0.f);
    }

    const MaterialShader* shader = scene->getShapeMaterialShader(its);

    // create the material closure
    MaterialClosure closure = MaterialClosure(sampler,
                                              &its,
                                              &ray,
                                              scene,
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

        Color3f recur = Li(scene, sampler, new_ray);

        return closure.albedo * (recur * 1.f / rr_cont_probability) +
               closure.nee + closure.emission;
    }

    return closure.emission + closure.nee;
}
