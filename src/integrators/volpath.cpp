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

// TODO: integrate clocker into volumetric rendering

VolPath_Integrator::VolPath_Integrator(FilterNode *filter,
                                       Integrator::Params *params)
    : Integrator(filter, params) {}

void VolPath_Integrator::preProcess(const Scene *scene, Sampler *sampler)
{
    Integrator::preProcess(scene, sampler);
}

Color3f VolPath_Integrator::Li(const Scene *scene,
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
    // TODO: maybe make this a process of the closure constructor
    closure.first_diffuse_evals = fb_emitter_samples;

    if (scene->env_medium_node)
    {
        closure.media = scene->env_medium_node->media;
    }

    // TODO: in the future support different bounce #'s by path types
    for (int bounces = 0; bounces < max_bounces; ++bounces)
    {
        if (beta.isZero())
            break;

        Intersection its;

        if (!scene->intersect_full(ray, its))
        {
            Li += beta * scene->env_emission(ray);
            break;
        }

        if (closure.media)
        {
            MediaClosure medium_closure(closure.media, ray.near, its.t);

            beta *= closure.media->sample(ray, sampler, medium_closure);

            if (medium_closure.handleScatter())
            {
                its.p = ray(medium_closure.sampled_t);

                closure.its = &its;
                closure.ray = &ray;
                closure.wi = -ray.dir;
                closure.emission = COLOR_BLACK;
                closure.nee = COLOR_BLACK;
                closure.albedo = COLOR_BLACK;

                // TODO: incorporate eval_multi_emitters here
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
                Float phase = closure.media->sample_phase(wo, wi, sampler->next2D());

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
                            Epsilon,
                            std::numeric_limits<Float>::infinity(),
                            ray.depth + 1);

                closure.last_spec = closure.is_specular;
                closure.is_specular = false;

                Float rr_prob = std::min(beta.maxValue(), ONE);

                Li += closure.nee;

                if (sampler->next1D() > rr_prob)
                    break;

                beta /= rr_prob;

                // the current media should not change during internal scattering
                continue;
            }
        }

        // special check to see if the object hit is null
        if (its.intersected_mesh->is_null)
        {
            // if (debug) std::cout << "intersecting boundry " << std::endl;
            const MediumBoundry *boundry = its.intersected_mesh->boundry;

            if (boundry)
            {
                // TODO: check if you are entering or exiting
                closure.wi = its.toLocal(-ray.dir);
                if (CoordinateFrame::cosTheta(closure.wi) <= 0)
                {
                    closure.media = (boundry->outside) ? closure.media = boundry->outside->media : nullptr;
                }
                else
                {
                    closure.media = (boundry->inside) ? closure.media = boundry->inside->media : nullptr;
                }
            }

            ray = Ray3f(its.p,
                        ray.dir,
                        Epsilon,
                        std::numeric_limits<Float>::infinity(),
                        ray.depth);
            bounces--;

            continue;
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

        Float rr_prob = std::min(beta.maxValue(), ONE);

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
        if (cosTerm < ZERO)
            cosTerm = -cosTerm;
        if (closure.is_specular)
            cosTerm = ONE;

        Li += beta * (closure.nee + closure.emission);
        beta *= closure.albedo * cosTerm / (closure.pdf * rr_prob);
    }

    return Li;
}

FEIGN_END()
