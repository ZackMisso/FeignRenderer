#pragma once

#include <integrator.h>

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
}
