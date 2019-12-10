/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
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
    Integrator(long max_time, long max_heuristic)
        : filter(nullptr),
          max_time(max_time),
          max_heuristic(max_heuristic) { }

    virtual ~Integrator() { }

    virtual void preProcess();

    virtual void render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const;

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const = 0;

protected:
    ReconstructionFilter* filter;
    long max_time;
    long max_heuristic;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Ambient Occlusion Integrator
/////////////////////////////////////////////////
class Ambient_Occlusion_Integrator : public Integrator
{
public:
    Ambient_Occlusion_Integrator(long max_time, long max_heuristic);

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
    Light_Unidirectional_Integrator(long max_time, long max_heuristic);

    virtual void preProcess();

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////


/////////////////////////////////////////////////
// Shading Normals Integrator
/////////////////////////////////////////////////
class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator(long max_time, long max_heuristic);

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
    Path_Bidirectional_Integrator(long max_time, long max_heuristic);

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
    Path_Unidirectional_Integrator(long max_time, long max_heuristic);

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
    WhittedIntegrator(long max_time, long max_heuristic);

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
    IntegratorNode(std::string name) : Node(name) { }
    IntegratorNode(Integrator* integrator) : integrator(integrator) { }
    IntegratorNode(Integrator* integrator, std::string name)
        : Node(name),
          integrator(integrator) { }

    Integrator* integrator;
};
/////////////////////////////////////////////////
