/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

FEIGN_BEGIN()

// TODO: for now this will only support homogeneous global media

VolPathTrans_Integrator::VolPathTrans_Integrator(FilterNode *filter,
                                                 Integrator::Params *params)
    : Integrator(filter, params) {}

void VolPathTrans_Integrator::preProcess(const Scene *scene, Sampler *sampler)
{
    Integrator::preProcess(scene, sampler);
}

// this integrator assumes that the medium being used is bounded
Color3f VolPathTrans_Integrator::Li(const Scene *scene,
                                    Sampler *sampler,
                                    const Ray3f &cam_ray,
                                    bool debug) const
{
    Color3f Li = Color3f(1.f);
    Color3f beta = Color3f(1.f);
    Ray3f ray = cam_ray;

    // predefine this so it does not have to get recreated every loop
    MaterialClosure closure = MaterialClosure(sampler,
                                              scene,
                                              false,
                                              true);

    Intersection its;

    // TODO: medium needs to be set at the end, not during intersection
    if (!scene->intersect_full(ray, its))
        return Li;

    // media_check
    // TODO: this will be broken if the camera starts inside the medium
    if (its.intersected_mesh->boundry)
    {
        closure.media = its.intersected_mesh->boundry->inside->media;

        ray = Ray3f(its.p,
                    ray.dir,
                    Epsilon,
                    std::numeric_limits<Float>::infinity(),
                    ray.depth);

        bool intersected = scene->intersect_full(ray, its);

        if (closure.media)
        {
            MediaClosure medium_closure(closure.media, ray.near, its.t);

            return closure.media->transmittance(ray, sampler, medium_closure);
        }
    }

    return Li;
}

FEIGN_END()
