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
class Integrator : public Node
{
public:
    Integrator();
    Integrator(Node* parent);
    virtual ~Integrator();

    virtual void preProcess();

    virtual void render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const;

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const = 0;

    virtual std::string getName() const;

    virtual NodeType getNodeType() const;

protected:
    ReconstructionFilter* filter;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Ambient Occlusion Integrator
/////////////////////////////////////////////////
class Ambient_Occlusion_Integrator : public Integrator
{
public:
    Ambient_Occlusion_Integrator();
    Ambient_Occlusion_Integrator(Node* parent);

    virtual void preProcess();

    virtual std::string getName() const;

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
    Light_Unidirectional_Integrator();
    Light_Unidirectional_Integrator(Node* parent);

    virtual void preProcess();

    virtual std::string getName() const;

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
    NormalIntegrator();
    NormalIntegrator(Node* parent);

    virtual void preProcess();

    virtual std::string getName() const;

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
    Path_Bidirectional_Integrator();
    Path_Bidirectional_Integrator(Node* parent);

    virtual void preProcess();

    virtual std::string getName() const;

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
    Path_Unidirectional_Integrator();
    Path_Unidirectional_Integrator(Node* parent);

    virtual void preProcess();

    virtual std::string getName() const;

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
    WhittedIntegrator();
    WhittedIntegrator(Node* parent);

    virtual void preProcess();

    virtual std::string getName() const;

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
/////////////////////////////////////////////////
