/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

// TODO: for now this will only support homogeneous global media

VolPathTrans_Integrator::VolPathTrans_Integrator(FilterNode* filter,
                                                 Integrator::Params* params)
    : Integrator(filter, params) { }

void VolPathTrans_Integrator::preProcess()
{
    Integrator::preProcess();
}

// this integrator assumes that the medium being used is bounded
Color3f VolPathTrans_Integrator::Li(const Scene* scene,
                                    Sampler* sampler,
                                    const Ray3f& cam_ray) const
{
    Color3f Li = Color3f(1.f);
    Color3f beta = Color3f(1.f);
    Ray3f ray = cam_ray;

    // predefine this so it does not have to get recreated every loop
    MaterialClosure closure = MaterialClosure(sampler,
                                              scene,
                                              false,
                                              true);

    // for (int bounces = 0; bounces < 15; ++bounces)
    // {
    //     // if (bounces != 0)
    //     //     LOG("Number of bounces: " + std::to_string(bounces));
    //
    //     if (beta.isZero()) break;
    //
    //     // LOG("bounce: " + std::to_string(bounces));
    //     // LOG("beta: ");
    //     // LOG(beta);
    //     // LOG("");

        Intersection its;

        // TODO: medium needs to be set at the end, not during intersection
        // LOG("initial intersect");
        if (!scene->intersect_full(ray, its)) return Li;

        // LOG("checking if null");
        // media_check
        // TODO: this will be broken for media inside glass
        if (its.intersected_mesh->is_null)
        {
            // LOG("getting shape medium");
            closure.media = scene->getShapeMedium(its);

            // LOG("creating ray");

            ray = Ray3f(its.p,
                        ray.dir,
                        Epsilon,
                        std::numeric_limits<Float>::infinity(),
                        ray.depth);

            // LOG("intersecting full again");
            bool intersected = scene->intersect_full(ray, its);

            if (closure.media)
            {
                // LOG("closure media defined");
                MediaClosure medium_closure(closure.media, ray.near, its.t);

                return closure.media->transmittance(ray, sampler, ray.near, its.t);
            }
        }

        //         // LOG("sampling medium");
        //         Li = closure.media->sample(ray, sampler, medium_closure);
        //
        //         return Li;
        //
        //         // beta *= closure.media->sample(ray, sampler, medium_closure);
        //         // LOG(val);
        //         beta *= val;
        //         // LOG("post sample medium");
        //         // LOG(beta);
        //
        //         if (medium_closure.handleScatter())
        //         {
        //             // sample lighting
        //             // Point3f actual_p = its.p;
        //             its.p = ray(medium_closure.sampled_t);
        //
        //             closure.its = &its;
        //             closure.ray = &ray;
        //             closure.wi = -ray.dir;
        //             closure.emission = COLOR_BLACK;
        //             closure.nee = COLOR_BLACK;
        //             closure.albedo = COLOR_BLACK;
        //
        //             if (closure.sample_all_emitters)
        //             {
        //                 scene->eval_all_emitters(closure, true);
        //             }
        //             else
        //             {
        //                 scene->eval_one_emitter(closure, true);
        //             }
        //
        //             Vector3f wo = -ray.dir;
        //             Vector3f wi;
        //             Float phase = closure.media->sample_phase(wo, wi, sampler->next2D());
        //             // phase = 1;
        //
        //             // LOG("phase: " + std::to_string(phase));
        //
        //             // maybe accumulate shadow rays in closure in the future
        //             for (int i = 0; i < closure.shadow_rays.size(); ++i)
        //             {
        //                 if (closure.shadow_rays[i].valid)
        //                 {
        //                     // Vector3f wo = shadow_rays[i].shadow_ray.normalized();
        //                     // The pdf of choosing a specific scattering direction
        //                     // should be equal to the evaluated phase... so they
        //                     // will end up canceling out.. though I am leaving this
        //                     // here to be incorporated with MIS in the future.
        //                     //
        //                     //shadow_rays[i].throughput *=
        //                     //    medium_closure.media->eval_phase(wo, ray.dir);
        //                     //
        //                     // LOG("throughput: ");
        //                     // LOG(closure.shadow_rays[i].throughput);
        //                     closure.nee += beta * phase * closure.shadow_rays[i].throughput;
        //                 }
        //             }
        //
        //             // LOG("prev dir: ");
        //             // LOG(ray.dir);
        //             // LOG("new dir: ");
        //             // LOG(wi);
        //
        //             ray = Ray3f(ray(medium_closure.sampled_t),
        //                         wi.normalized(),
        //                         Epsilon,
        //                         std::numeric_limits<Float>::infinity(),
        //                         ray.depth + 1);
        //
        //             closure.last_spec = closure.is_specular;
        //             closure.is_specular = false;
        //
        //             Float rr_prob = std::min(beta.maxValue(), 1.f);
        //
        //             Li += closure.nee;
        //
        //             if (sampler->next1D() > rr_prob) break;
        //
        //             beta /= rr_prob;
        //
        //             continue;
        //         }
        //
        //         if (!intersected) break;
        //     }
        //     else
        //     {
        //         if (!intersected) break;
        //
        //         // LOG("skipping null object");
        //         ray = Ray3f(its.p,
        //                     ray.dir,
        //                     Epsilon,
        //                     std::numeric_limits<Float>::infinity(),
        //                     ray.depth);
        //
        //         continue;
        //     }
        // }

        // if (closure->media)
        // {

        // TODO: how to create shaders out of a medium?
        // MediaClosure medium_closure(closure.medium,
        //                             its.t);

        // medium should be set by closure since not all materials refract
        // if (its.medium)
        // {
        //     ray.far = its.t;
        //     // LOG("medium exists");
        //     beta *= its.medium->sample(ray, sampler, medium_closure);
        //     // LOG("medium sample");
        // }

        // if (closure.media)
        // {
        //     ray.far = its.t;
        //     // LOG("beta: " + std::to_string(beta(0)));
        //     beta *= closure.media->sample(ray, sampler, medium_closure);
        //     // LOG("beta 2: " + std::to_string(beta(0)));
        //
        //     if (beta.isZero()) break;
        // }

        // if (medium_closure.handleScatter())
        // {
        //     // TODO - will be done later
        // }
        // else
        // {
        //     if (its.intersected_mesh->is_null)
        //     {
        //         ray = Ray3f(its.p,
        //                     ray.dir,
        //                     Epsilon,
        //                     std::numeric_limits<Float>::infinity(),
        //                     ray.depth);
        //
        //         continue;
        //     }
        //
        //     // assert(false);
        //
        //     const MaterialShader* shader = scene->getShapeMaterialShader(its);
        //
        //     closure.its = &its;
        //     closure.ray = &ray;
        //     closure.wi = its.toLocal(-ray.dir);
        //     closure.emission = COLOR_BLACK;
        //     closure.nee = COLOR_BLACK;
        //     closure.albedo = COLOR_BLACK;
        //
        //     // evaluate the material shader
        //     shader->evaluate(closure);
        //
        //     // accumulate the shadow rays
        //     closure.accumulate_shadow_rays(shader);
        //
        //     Float rr_prob = std::min(beta.maxValue(), 1.f);
        //
        //     // random termination
        //     if (sampler->next1D() > rr_prob)
        //     {
        //         Li += beta * (closure.emission + closure.nee);
        //         break;
        //     }
        //
        //     // sample the next path
        //     closure.wi = its.toLocal(-ray.dir);
        //     shader->sample(closure);
        //
        //     if (closure.pdf == 0.f)
        //     {
        //         Li += beta * (closure.emission + closure.nee);
        //         break;
        //     }
        //
        //     ray = Ray3f(its.p,
        //                 its.toWorld(closure.wo),
        //                 Epsilon,
        //                 std::numeric_limits<Float>::infinity(),
        //                 ray.depth + 1);
        //
        //     Float cosTerm = its.s_frame.n % ray.dir;
        //     if (cosTerm < 0.f) cosTerm = -cosTerm;
        //     if (closure.is_specular) cosTerm = 1.f;
        //
        //     Li += beta * (closure.nee + closure.emission);
        //     beta *= closure.albedo * cosTerm / (closure.pdf * rr_prob);
        //
        //     // TODO: replace this check with an inside/outside/counter
        //     // if there is no active medium, but a global medium exists,
        //     // set the active medium to the global medium
        //     // if (!closure.media)
        //     // {
        //     //     closure.media = scene->env_medium_node->media;
        //     // }
        // // }
    // }

    return Li;
}
