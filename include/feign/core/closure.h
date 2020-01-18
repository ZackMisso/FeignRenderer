/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/ray.h>
#include <feign/misc/intersection.h>

// yes i know this is bad design
class Scene;
class Sampler;
class MaterialShader;

// this struct represents what is returned from the material shader when it
// gets evaluated
struct MaterialClosure
{
    MaterialClosure();

    MaterialClosure(Sampler* sampler,
                    const Intersection* its,
                    const Ray3f* ray,
                    const Scene* scene,
                    bool sample_all_emitters = false);

    void accumulate_shadow_rays(const MaterialShader* shader);

    // always inputs
    Sampler* sampler;        // the sampler to use
    const Intersection* its; // a reference to the intersection info
    const Ray3f* ray;        // a reference to the incoming ray
    const Scene* scene;      // a reference to the scene
    Vector3f wi;

    // sometimes inputs or outputs depending on sampling
    Vector3f wo;             // the outgoing ray
    std::vector<EmitterEval> shadow_rays;

    // always outputs
    Color3f albedo;          // the bsdf sample of the material
    Color3f emission;        // the emissive term from the material
    Color3f nee;             // contribution from nee
    Float pdf;               // the pdf of the bsdf sample
    Float eta;               // how the ior changes
    bool is_specular;        // whether or not this material is discretely evaluated
    // bool trace_ray;
    bool sample_all_emitters;
};

// // this struct represents what is returned from a media shader when it gets
// // evaluated
// struct MediaClosure
// {
//     // TODO
// };
