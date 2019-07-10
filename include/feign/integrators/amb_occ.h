#pragma once

#include <feign/integrators/integrator.h>

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
