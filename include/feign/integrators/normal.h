#pragma once

#include <feign/integrators/integrator.h>

class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator();
    NormalIntegrator(Node* parent);

    virtual void preProcess();

    virtual string getName() const;

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
