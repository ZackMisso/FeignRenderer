/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/camera.h>
#include <feign/core/sampler.h>
#include <feign/core/recon_filter.h>
#include <feign/core/accel_photons.h>
#include <feign/misc/render_pool.h>

FEIGN_BEGIN()

// needed forward declaration
class Scene;

/////////////////////////////////////////////////
// Integrator
/////////////////////////////////////////////////
class Integrator
{
public:
    struct Params
    {
        Params(long max_time,
               long max_heuristic,
               std::string location,
               int max_bounces = 10,
               int fb_emitter_samples = 1,
               bool verbose = false)
            : max_time(max_time),
              max_heuristic(max_heuristic),
              location(location),
              max_bounces(max_bounces),
              fb_emitter_samples(fb_emitter_samples),
              verbose(verbose) {}

        long max_time;
        long max_heuristic;
        int max_bounces;
        // set the number of first bounce emitter samples to a custom value to
        // speed up convergence for specific scenes
        int fb_emitter_samples;
        std::string location;
        // bool eval_all_emitters; // TODO: in a future update make this
        //       configurable
        bool verbose;
    };

    Integrator() {} // default constructor should never be used, directly by
                    // the api, only for tests or temporary initializations

    Integrator(FilterNode *filter,
               Integrator::Params *params)
        : filter(filter),
          location(params->location),
          max_time(params->max_time),
          max_heuristic(params->max_heuristic),
          max_bounces(params->max_bounces),
          fb_emitter_samples(params->fb_emitter_samples),
          verbose(params->verbose) {}

    virtual ~Integrator() {}

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual void render(const Scene *scene,
                        const Camera *camera,
                        Sampler *sampler,
                        Imagef &image) const;

    // multithreaded implementation
    virtual void render_fast(const Scene *scene,
                             const Camera *camera,
                             Sampler *sampler,
                             Imagef &image) const;

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const = 0;

    std::string location;
    FilterNode *filter;

protected:
    long max_time;
    long max_heuristic;
    int max_bounces;
    int fb_emitter_samples;
    bool verbose;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Ambient Occlusion Integrator
/////////////////////////////////////////////////
class Ambient_Occlusion_Integrator : public Integrator
{
public:
    Ambient_Occlusion_Integrator(FilterNode *filter,
                                 Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Unidirectional Light Tracer
/////////////////////////////////////////////////
class Light_Unidirectional_Integrator : public Integrator
{
public:
    Light_Unidirectional_Integrator(FilterNode *filter,
                                    Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

// This integrator is used for debugging
/////////////////////////////////////////////////
// Shading Normals Integrator
/////////////////////////////////////////////////
class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator(FilterNode *filter,
                     Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

// Produces nicer looking results
/////////////////////////////////////////////////
// Nice looking Shading Normals Integrator
/////////////////////////////////////////////////
class NiceNormalIntegrator : public Integrator
{
public:
    NiceNormalIntegrator(FilterNode *filter,
                         Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Bidirectional Path Tracer
/////////////////////////////////////////////////
class Path_Bidirectional_Integrator : public Integrator
{
public:
    Path_Bidirectional_Integrator(FilterNode *filter,
                                  Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Path Tracer
/////////////////////////////////////////////////
class Path_Unidirectional_Integrator : public Integrator
{
public:
    Path_Unidirectional_Integrator(FilterNode *filter,
                                   Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Volumetric Path Tracer
/////////////////////////////////////////////////
class VolPath_Integrator : public Integrator
{
public:
    VolPath_Integrator(FilterNode *filter,
                       Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Volumetric Path Tracer - with support for non-exponential media
/////////////////////////////////////////////////
class VolPathNonExp_Integrator : public Integrator
{
public:
    VolPathNonExp_Integrator(FilterNode *filter,
                             Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Volumetric Transmittance Integrator
// used for debugging, just returns transmittance
/////////////////////////////////////////////////
class VolPathTrans_Integrator : public Integrator
{
public:
    VolPathTrans_Integrator(FilterNode *filter,
                            Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Photon Mapping Integrator
/////////////////////////////////////////////////
class PhotonMapping : public Integrator
{
public:
    struct Params : public Integrator::Params
    {
        Params(long max_time,
               long max_heuristic,
               std::string location,
               int num_photons,
               int max_bounces = 10,
               bool verbose = false)
            : Integrator::Params(max_time,
                                 max_heuristic,
                                 location,
                                 max_bounces,
                                 verbose),
              num_photons(num_photons) {}

        Integrator::Params convert_to_base()
        {
            return Integrator::Params(max_time,
                                      max_heuristic,
                                      location,
                                      max_bounces,
                                      verbose);
        }

        int num_photons;
        long max_time;
        long max_heuristic;
        int max_bounces;
        std::string location;
        // bool eval_all_emitters; // TODO: in a future update make this
        //       configurable
        bool verbose;
    };

    PhotonMapping(FilterNode *filter,
                  Params *params);

    ~PhotonMapping();

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual void scatter_photons(const Scene *scene,
                                 Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;

    // virtual void render(const Scene* scene,
    //                     const Camera* camera,
    //                     Sampler* sampler,
    //                     Imagef& image) const;
    //
    // // multithreaded implementation
    // virtual void render_fast(const Scene* scene,
    //                          const Camera* camera,
    //                          Sampler* sampler,
    //                          Imagef& image) const;

    // TODO: create an actual acceleration structure to store photons
    PhotonAccel *photon_storage;
    int num_photons;
};

/////////////////////////////////////////////////
// Sandbox Integrator
// used for debugging or a temporary place for reworking current integrators
/////////////////////////////////////////////////
class Sandbox_Integrator : public Integrator
{
public:
    Sandbox_Integrator(FilterNode *filter,
                       Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Whitted Integrator
/////////////////////////////////////////////////
class WhittedIntegrator : public Integrator
{
public:
    WhittedIntegrator(FilterNode *filter,
                      Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Integrator Node structure
/////////////////////////////////////////////////
struct IntegratorNode : public Node
{
public:
    IntegratorNode() : integrator(nullptr) {}
    IntegratorNode(std::string name) : Node(name), integrator(nullptr) {}
    IntegratorNode(Integrator *integrator) : integrator(integrator) {}
    IntegratorNode(Integrator *integrator, std::string name)
        : Node(name),
          integrator(integrator) {}

    ~IntegratorNode() { delete integrator; }

    Integrator *integrator;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/////////////////////////////////////////////////
// Random integrators for making interesting images
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Cosine Term Integrator
/////////////////////////////////////////////////
class CosineTermIntegrator : public Integrator
{
public:
    CosineTermIntegrator(FilterNode *filter,
                         Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// this integrator is used for experimenting with implementing
// a wireframe material. Seeing if you can use barycentric
// coordinates to tell when you are close to an edge or not.
/////////////////////////////////////////////////
// Barycentric Integrator
/////////////////////////////////////////////////
class BarycentricIntegrator : public Integrator
{
public:
    BarycentricIntegrator(FilterNode *filter,
                          Integrator::Params *params);

    virtual void preProcess(const Scene *scene,
                            Sampler *sampler);

    virtual Color3f Li(const Scene *scene,
                       Sampler *sampler,
                       const Ray3f &ray,
                       bool debug = false) const;
};
/////////////////////////////////////////////////

FEIGN_END()
