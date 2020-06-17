/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/camera.h>
#include <feign/core/sampler.h>
#include <feign/core/recon_filter.h>

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
               bool verbose = false)
            : max_time(max_time),
              max_heuristic(max_heuristic),
              location(location),
              max_bounces(max_bounces),
              verbose(verbose) { }

        long max_time;
        long max_heuristic;
        int max_bounces;
        std::string location;
        // bool eval_all_emitters; // TODO: in a future update make this
                                   //       configurable
        bool verbose;
    };

    struct RenderTile
    {
        RenderTile(int min_x, int min_y, int max_x, int max_y)
            : min_x(min_x),
              min_y(min_y),
              max_x(max_x),
              max_y(max_y)
        {
            int size = (max_x - min_x) * (max_y - min_y);
            pixels = std::vector<Color3f>(size);
            weights = std::vector<Float>(size);
        }

        // void render(const Scene* scene,
        //             const Camera* camera,
        //             const ReconstructionFilter* filter,
        //             Sampler* sampler);

        void add_radiance(Color3f rad, int i, int j);
        void add_weight(Float wei, int i, int j);
        void finalize(Imagef& image);

        std::vector<Color3f> pixels;
        std::vector<Float> weights;
        // this is not inclusive
        int min_x;
        int min_y;
        int max_x;
        int max_y;
    };

    Integrator(FilterNode* filter,
               Integrator::Params* params)
        : filter(filter),
          location(params->location),
          max_time(params->max_time),
          max_heuristic(params->max_heuristic),
          max_bounces(params->max_bounces),
          verbose(params->verbose) { }

    virtual ~Integrator() { }

    virtual void preProcess();

    virtual void render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const;

    virtual void renderTile(const Scene* scene,
                            const Camera* camera,
                            Sampler* sampler,
                            RenderTile& tile) const;

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const = 0;

    std::string location;
protected:
    FilterNode* filter;
    long max_time;
    long max_heuristic;
    int max_bounces;
    bool verbose;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Ambient Occlusion Integrator
/////////////////////////////////////////////////
class Ambient_Occlusion_Integrator : public Integrator
{
public:
    Ambient_Occlusion_Integrator(FilterNode* filter,
                                 Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Unidirectional Light Tracer
/////////////////////////////////////////////////
class Light_Unidirectional_Integrator : public Integrator
{
public:
    Light_Unidirectional_Integrator(FilterNode* filter,
                                    Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

// This integrator is used for debugging
/////////////////////////////////////////////////
// Shading Normals Integrator
/////////////////////////////////////////////////
class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator(FilterNode* filter,
                     Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

// Produces nicer looking results
/////////////////////////////////////////////////
// Nice looking Shading Normals Integrator
/////////////////////////////////////////////////
class NiceNormalIntegrator : public Integrator
{
public:
    NiceNormalIntegrator(FilterNode* filter,
                         Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Bidirectional Path Tracer
/////////////////////////////////////////////////
class Path_Bidirectional_Integrator : public Integrator
{
public:
    Path_Bidirectional_Integrator(FilterNode* filter,
                                  Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Path Tracer
/////////////////////////////////////////////////
class Path_Unidirectional_Integrator : public Integrator
{
public:
    Path_Unidirectional_Integrator(FilterNode* filter,
                                   Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Volumetric Path Tracer
/////////////////////////////////////////////////
class VolPath_Integrator : public Integrator
{
public:
    VolPath_Integrator(FilterNode* filter,
                       Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Whitted Integrator
/////////////////////////////////////////////////
class WhittedIntegrator : public Integrator
{
public:
    WhittedIntegrator(FilterNode* filter,
                      Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Integrator Node structure
/////////////////////////////////////////////////
struct IntegratorNode : public Node
{
public:
    IntegratorNode() : integrator(nullptr) { }
    IntegratorNode(std::string name) : Node(name), integrator(nullptr) { }
    IntegratorNode(Integrator* integrator) : integrator(integrator) { }
    IntegratorNode(Integrator* integrator, std::string name)
        : Node(name),
          integrator(integrator) { }

    ~IntegratorNode() { delete integrator; }

    Integrator* integrator;
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
    CosineTermIntegrator(FilterNode* filter,
                         Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
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
    BarycentricIntegrator(FilterNode* filter,
                          Integrator::Params* params);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////
