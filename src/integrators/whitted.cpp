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

    // const std::vector<Emitter>* emitters = scene->emitters;
    const MaterialShader* shader = scene->getShapeMaterialShader(its);

    // create the material closure
    MaterialClosure closure = MaterialClosure(sampler,
                                              &its,
                                              &ray,
                                              scene,
                                              true);

    // TODO: shouldn't these all go in one call?
    // evaluate the material shader
    shader->evaluate(closure);

    // accumulate the shadow rays
    closure.accumulate_shadow_rays(shader);

    if (closure.is_specular)
    {
        // assert(false);
        // sample the next path
        closure.wi = its.toLocal(-ray.dir);
        shader->sample(closure);

        // random termination
        if (sampler->next1D() > rr_cont_probability) return Color3f(0.f);

        // assert(false);

        Ray3f new_ray(its.p,
                      its.toWorld(closure.wo),
                      Epsilon,
                      std::numeric_limits<Float>::infinity(),
                      ray.depth + 1);

        Color3f recur = Li(scene, sampler, new_ray);
        // LOG("recur:", recur);
        // LOG("nee:", closure.nee);
        // LOG("albedo:", closure.albedo);

        // return closure.nee + closure.emission;

        return closure.albedo * (recur * 1.f / rr_cont_probability) +
               closure.nee + closure.emission;
    }

    return closure.emission + closure.nee;

    // do stuff

    // first evaluate the material for all emitters
    // if (!material->isDelta())
    // {
    //
    // }

    throw new FeignRendererException("zack broke all of this");

    return Color3f(0.f);

    // const std::vector<Emitter*> emitters = scene->emitters;
    // // Color3f mat_scale = Color3f(1.f);
    // // const BSDF* bsdf = scene->getShapeBSDF(its, mat_scale);
    //
    // Color3f result(0.f);
    //
    //
    //
    // // TODO: create a method to sample all emitters in base integrator
    // if (!bsdf->isDelta()) // TODO: is this the best way of handling this
    // {
    //     for (int i = 0; i < emitters.size(); ++i)
    //     {
    //         // sample emitter
    //         EmitterQuery eqr(its.p);
    //         Float* emitter_pdf = nullptr;
    //         Color3f Li = emitters[i]->sample_li(eqr,
    //                                             sampler->next2D(),
    //                                             pdf);
    //
    //         BSDFQuery bqr(its.toLocal(-dir),
    //                       its.toLocal(eqr.wi),
    //                       its.uv,
    //                       its.p);
    //
    //         Color3f bsdf_val = bsdf->eval(bqr) * mat_scale;
    //         float cos_term = its.s_frame.n % eqr.wi;
    //
    //         if (cos_term < -Epsilon) cos_term = -cos_term;
    //
    //         Ray3f shadow_ray(its.p,
    //                          eqr.wi,
    //                          Epsilon,
    //                          sqrt(eqr.sqr_dist));
    //
    //         Intersection tmp;
    //
    //         if (!scene->intersect(shadow_ray, tmp) || global_params.ignore_shadow_checks)
    //         {
    //             result += bsdf_val * Li * cos_term;
    //         }
    //     }
    // }
    // else
    // {
    //     // perfect specular reflections
    //
    //     // perform russian roulette first to save computation
    //     if (sampler->next1D() > rr_cont_probability) return Color3f(0.f);
    //
    //     BSDFQuery bqr(its.toLocal(-dir),
    //                   its.uv,
    //                   its.p);
    //
    //     Point2f sample = sampler->next2D();
    //     Color3f color = bsdf->sample(bqr, sample) * mat_scale;
    //
    //     if (color.is_black()) return color;
    //
    //     Ray3f reflection(its.p,
    //                      its.toWorld(bqr.wo),
    //                      Epsilon,
    //                      std::numeric_limits<Float>::infinity(),
    //                      ray.depth+1);
    //
    //     Color3f recur = Li(scene, sampler, reflection);
    //
    //     return recur * 1.f / rr_cont_probability * color;
    //
    //     // TODO: add refraction
    // }
    //
    // return result;







    // Intersection its;
    //
    // Vector3f dir = ray.dir;
    // Float rr_cont_probability = 0.95f;
    //
    // if (!scene->intersect(ray, its))
    // {
    //     return Color3f(0.f);
    // }
    //
    // const std::vector<Emitter*> emitters = scene->emitters;
    // Color3f mat_scale = Color3f(1.f);
    // const BSDF* bsdf = scene->getShapeBSDF(its, mat_scale);
    //
    // Color3f result(0.f);
    //
    // // TODO: create a method to sample all emitters in base integrator
    // if (!bsdf->isDelta()) // TODO: is this the best way of handling this
    // {
    //     for (int i = 0; i < emitters.size(); ++i)
    //     {
    //         EmitterQuery eqr(its.p);
    //
    //         Float* pdf = nullptr;
    //         Color3f Li = emitters[i]->sample_li(eqr,
    //                                             sampler->next2D(),
    //                                             pdf);
    //
    //         BSDFQuery bqr(its.toLocal(-dir),
    //                       its.toLocal(eqr.wi),
    //                       its.uv,
    //                       its.p);
    //
    //         Color3f bsdf_val = bsdf->eval(bqr) * mat_scale;
    //         float cos_term = its.s_frame.n % eqr.wi;
    //
    //         if (cos_term < -Epsilon) cos_term = -cos_term;
    //
    //         Ray3f shadow_ray(its.p,
    //                          eqr.wi,
    //                          Epsilon,
    //                          sqrt(eqr.sqr_dist));
    //
    //         Intersection tmp;
    //
    //         if (!scene->intersect(shadow_ray, tmp) || global_params.ignore_shadow_checks)
    //         {
    //             result += bsdf_val * Li * cos_term;
    //         }
    //     }
    // }
    // else
    // {
    //     // perfect specular reflections
    //
    //     // perform russian roulette first to save computation
    //     if (sampler->next1D() > rr_cont_probability) return Color3f(0.f);
    //
    //     BSDFQuery bqr(its.toLocal(-dir),
    //                   its.uv,
    //                   its.p);
    //
    //     Point2f sample = sampler->next2D();
    //     Color3f color = bsdf->sample(bqr, sample) * mat_scale;
    //
    //     if (color.is_black()) return color;
    //
    //     Ray3f reflection(its.p,
    //                      its.toWorld(bqr.wo),
    //                      Epsilon,
    //                      std::numeric_limits<Float>::infinity(),
    //                      ray.depth+1);
    //
    //     Color3f recur = Li(scene, sampler, reflection);
    //
    //     return recur * 1.f / rr_cont_probability * color;
    //
    //     // TODO: add refraction
    // }
    //
    // return result;
}
