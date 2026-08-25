/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

// THIS INTEGRATOR IS USED AS A SANDBOX FOR ACTIVE DEBUGGING
// what was I doing here lmao???

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

FEIGN_BEGIN()

// TODO: for now this will only support homogeneous global media

Sandbox_Integrator::Sandbox_Integrator(FilterNode *filter,
                                       Integrator::Params *params)
    : Integrator(filter, params) {}

void Sandbox_Integrator::pre_process(const Scene *scene, Sampler *sampler)
{
    Integrator::pre_process(scene, sampler);
}

Color3f Sandbox_Integrator::Li(const Scene *scene,
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

    for (int bounces = 0; bounces < 1; ++bounces)
    {
        if (beta.is_zero())
            break;

        Intersection its;

        // TODO: medium needs to be set at the end, not during intersection
        if (!scene->intersect_full(ray, its))
            break;

        // media_check
        // TODO: this will be broken for media inside glass
        // TODO: this is broken for multiple bounces
        if (its.intersected_mesh->is_null)
        {
            closure.media = its.intersected_mesh->boundry->inside->media;

            ray = Ray3f(its.p,
                        ray.dir,
                        EPSILON,
                        std::numeric_limits<Float>::infinity(),
                        ray.depth);

            bool intersected = scene->intersect_full(ray, its);

            if (closure.media)
            {
                MediaClosure medium_closure(closure.media, ray.near, its.t);

                Color3f val = closure.media->sample(ray, sampler, medium_closure);
                beta *= val;

                if (medium_closure.handle_scatter())
                {
                    // sample lighting
                    its.p = ray(medium_closure.sampled_t);

                    closure.its = &its;
                    closure.ray = &ray;
                    closure.wi = -ray.dir;
                    closure.emission = COLOR_BLACK;
                    closure.nee = COLOR_BLACK;
                    closure.albedo = COLOR_BLACK;

                    if (closure.sample_all_emitters)
                    {
                        scene->eval_all_emitters(closure, true);
                    }
                    else
                    {
                        scene->eval_one_emitter(closure, true);
                    }

                    Vector3f wo = -ray.dir;
                    Vector3f wi;
                    Float phase = closure.media->sample_phase(wo, wi, sampler->next_2d());

                    // maybe accumulate shadow rays in closure in the future
                    for (int i = 0; i < closure.shadow_rays.size(); ++i)
                    {
                        if (closure.shadow_rays[i].valid)
                        {
                            // Vector3f wo = shadow_rays[i].shadow_ray.normalized();
                            // The pdf of choosing a specific scattering direction
                            // should be equal to the evaluated phase... so they
                            // will end up canceling out.. though I am leaving this
                            // here to be incorporated with MIS in the future.
                            closure.nee += beta * phase * closure.shadow_rays[i].throughput;
                        }
                    }

                    ray = Ray3f(ray(medium_closure.sampled_t),
                                wi.normalized(),
                                EPSILON,
                                std::numeric_limits<Float>::infinity(),
                                ray.depth + 1);

                    closure.last_spec = closure.is_specular;
                    closure.is_specular = false;

                    Float rr_prob = std::min(beta.max_value(), ONE);

                    Li += closure.nee;

                    if (sampler->next_1d() > rr_prob)
                        break;

                    beta /= rr_prob;

                    continue;
                }

                if (!intersected)
                    break;
            }
            else
            {
                if (!intersected)
                    break;

                ray = Ray3f(its.p,
                            ray.dir,
                            EPSILON,
                            std::numeric_limits<Float>::infinity(),
                            ray.depth);

                continue;
            }
        }
        if (its.intersected_mesh->is_null)
        {
            ray = Ray3f(its.p,
                        ray.dir,
                        EPSILON,
                        std::numeric_limits<Float>::infinity(),
                        ray.depth);

            continue;
        }

        const MaterialShader *shader = scene->get_shape_material_shader(its);

        closure.its = &its;
        closure.ray = &ray;
        closure.wi = its.to_local(-ray.dir);
        closure.emission = COLOR_BLACK;
        closure.nee = COLOR_BLACK;
        closure.albedo = COLOR_BLACK;

        // evaluate the material shader
        shader->evaluate(closure);

        // accumulate the shadow rays
        closure.accumulate_shadow_rays(shader);

        Float rr_prob = std::min(beta.max_value(), ONE);

        // random termination
        if (sampler->next_1d() > rr_prob)
        {
            Li += beta * (closure.emission + closure.nee);
            break;
        }

        // sample the next path
        closure.wi = its.to_local(-ray.dir);
        shader->sample(closure);

        if (closure.pdf == ZERO)
        {
            Li += beta * (closure.emission + closure.nee);
            break;
        }

        ray = Ray3f(its.p,
                    its.to_world(closure.wo),
                    EPSILON,
                    std::numeric_limits<Float>::infinity(),
                    ray.depth + 1);

        Float cos_term = its.s_frame.n % ray.dir;
        if (cos_term < ZERO)
            cos_term = -cos_term;
        if (closure.is_specular)
            cos_term = ONE;

        Li += beta * (closure.nee + closure.emission);
        beta *= closure.albedo * cos_term / (closure.pdf * rr_prob);
    }

    return Li;
}

FEIGN_END()
