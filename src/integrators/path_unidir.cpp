/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

Path_Unidirectional_Integrator::Path_Unidirectional_Integrator(FilterNode* filter,
                                                               std::string location,
                                                               long max_time,
                                                               long max_heuristic)
    : Integrator(filter, location, max_time, max_heuristic) { }

void Path_Unidirectional_Integrator::preProcess()
{
    Integrator::preProcess();
}

Color3f Path_Unidirectional_Integrator::Li(const Scene* scene,
                                           Sampler* sampler,
                                           const Ray3f& ray) const
{
    Intersection its;

    Vector3f dir = ray.dir;
    Float rr_cont_probability = 0.95f;

    if (!scene->intersect(ray, its) || ray.depth > 10)
    {
        return Color3f(0.f);
    }

    const MaterialShader* shader = scene->getShapeMaterialShader(its);

    // create the material closure
    MaterialClosure closure = MaterialClosure(sampler,
                                              &its,
                                              &ray,
                                              scene,
                                              false);

    // evaluate the material shader
    shader->evaluate(closure);

    // accumulate the shadow rays
    closure.accumulate_shadow_rays(shader);

    // sample the next path
    closure.wi = its.toLocal(-ray.dir);
    shader->sample(closure);

    // random termination
    if (sampler->next1D() > rr_cont_probability ||
        closure.pdf == 0.f)
    {
        return closure.emission + closure.nee;
    }

    Ray3f new_ray(its.p,
                  its.toWorld(closure.wo),
                  Epsilon,
                  std::numeric_limits<Float>::infinity(),
                  ray.depth + 1);

    Float cosTerm = its.s_frame.n % new_ray.dir;
    if (cosTerm < 0.f) cosTerm = -cosTerm;

    Color3f recur = Li(scene, sampler, new_ray);

    return closure.albedo * recur * cosTerm /
           (closure.pdf * rr_cont_probability) +
           closure.nee + closure.emission;
}
