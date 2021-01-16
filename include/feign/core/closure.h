/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/media/media.h>
#include <feign/math/ray.h>
#include <feign/misc/intersection.h>

FEIGN_BEGIN()

class Scene;
class Sampler;
class MaterialShader;
class Media;

struct EmitterEval
{
    Color3f throughput;
    Vector3f shadow_ray;
    bool valid = false;
};

enum VertexType
{
    VERTEX_MEDIUM,
    VERTEX_DIFFUSE,
    VERTEX_SPECULAR
};

// TODO: clean this up a bit maybe?
// TODO: maybe create extended versions for non-exponential support + keeping
//       track of different types of paths
//
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
    Vector3f wi;
    const Scene* scene;      // a reference to the scene
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
    int first_diffuse_evals; // TODO: this should not have to be stored here
    bool is_specular;        // whether or not this material is discretely evaluated
    bool last_spec;          // whether the last evaluation was specular
    // bool trace_ray;
    bool sample_all_emitters;
    bool material_accepts_shadows;
    bool first_diffuse_event;

    // flags
    // 0 - diffuse
    // 1 - specular
    // 2 - medium
    uint32_t last_event;
};

struct PhotonTracingClosure
{
    PhotonTracingClosure();

    PhotonTracingClosure(Sampler* sampler,
                         Intersection* its,
                         Ray3f* ray,
                         const Scene* scene);

    Sampler* sampler;        // the sampler to use
    Intersection* its; // a reference to the intersection info
    Ray3f* ray;        // a reference to the incoming ray
    const Scene* scene;      // a reference to the scene
    Vector3f wi;
    const Media* media;

    Vector3f wo;             // the outgoing ray
    Float pdf;               // the pdf of the bsdf sample
    Color3f albedo;          // blah
};

struct PhotonGatherClosure
{
    // TODO
};

// this struct represents what is returned from a media shader when it gets
// evaluated
struct MediaClosure
{
    MediaClosure(const Media* medium,
                 Float t_min,
                 Float t_max)
        : medium(medium),
          t_min(t_min),
          t_max(t_max),
          sampled_t(t_max) { }

    MediaClosure(const Media* medium,
                 Float t_min,
                 Float t_max,
                 uint32_t last_event,
                 uint32_t next_event)
        : medium(medium),
          t_min(t_min),
          t_max(t_max),
          sampled_t(t_max),
          last_event(last_event),
          next_event(next_event) { }

    bool handleScatter()
    {
        return sampled_t < t_max;
    }

    // input
    const Media* medium;
    Float t_max;
    Float t_min;

    // TODO: allow for multiple sampled t's
    // output
    Float sampled_t;
    Color3f nee;

    // TODO: turn these into flags
    // used for non-exponential media
    uint32_t last_event;
    uint32_t next_event;
};

FEIGN_END()
