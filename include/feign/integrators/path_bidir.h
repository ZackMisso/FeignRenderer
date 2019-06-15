#pragma once

#include <feign/integrators/integrator.h>

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
