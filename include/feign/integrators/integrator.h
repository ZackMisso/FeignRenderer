#pragma once

#include <feign/node.h>
#include <feign/cameras/camera.h>
#include <feign/samplers/sampler.h>

// needed forward declaration
class Scene;

class Integrator : public Node
{
public:
    Integrator();
    Integrator(Node* parent);
    virtual ~Integrator();

    virtual void render(const Scene* scene,
                        const Camera* camera,
                        Sampler* sampler,
                        Imagef& image) const;

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f ray) const = 0;

    virtual string getName() const;

    virtual NodeType getNodeType() const;
};
