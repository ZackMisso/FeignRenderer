#pragma once

#include <feign/node.h>
#include <feign/cameras/camera.h>
#include <feign/samplers/sampler.h>
#include <feign/filters/recon_filter.h>

// needed forward declaration
class Scene;

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
