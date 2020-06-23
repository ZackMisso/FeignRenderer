/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/media/media.h>
#include <feign/math/ray.h>
#include <feign/misc/intersection.h>

// yes i know this is bad design
class Scene;
class Sampler;
class MaterialShader;
class Media;

// this struct represents what is returned from the material shader when it
// gets evaluated
struct MaterialClosure
{
    MaterialClosure();

    MaterialClosure(Sampler* sampler,
                    Intersection* its,
                    Ray3f* ray,
                    const Scene* scene,
                    bool sample_all_emitters = false,
                    bool last_bounce_specular = false);

    MaterialClosure(Sampler* sampler,
                    const Scene* scene,
                    bool sample_all_emitters = false,
                    bool last_bounce_specular = false);

    // TODO: why is this a separate operation
    void accumulate_shadow_rays(const MaterialShader* shader);

    // always inputs
    Sampler* sampler;        // the sampler to use
    Intersection* its; // a reference to the intersection info
    Ray3f* ray;        // a reference to the incoming ray
    const Scene* scene;      // a reference to the scene
    Vector3f wi;
    const Media* media;

    // sometimes inputs or outputs depending on sampling
    Vector3f wo;             // the outgoing ray
    std::vector<EmitterEval> shadow_rays; // TODO: get rid of this

    // always outputs
    Color3f albedo;          // the bsdf sample of the material
    Color3f emission;        // the emissive term from the material
    Color3f nee;             // contribution from nee
    Float pdf;               // the pdf of the bsdf sample
    Float eta;               // how the ior changes
    bool is_specular;        // whether or not this material is discretely evaluated
    bool last_spec;          // whether the last evaluation was specular
    // bool trace_ray;
    bool sample_all_emitters;
};

// this struct represents what is returned from a media shader when it gets
// evaluated
struct MediaClosure
{
    MediaClosure(const Media* medium,
                 Float max_t)
        : medium(medium),
          max_t(max_t),
          sampled_t(max_t) { }

    bool handleScatter()
    {
        return sampled_t < max_t;
    }

    // input
    const Media* medium;
    Float max_t;

    // output
    Float sampled_t;
};
