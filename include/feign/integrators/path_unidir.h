#pragma once

#include <feign/integrators/integrator.h>

class Path_Unidirectional_Integrator : public Integrator
{
public:
    Path_Unidirectional_Integrator();
    Path_Unidirectional_Integrator(Node* parent);

    virtual void preProcess(bool use_prims);

    virtual std::string getName() const;

    virtual Color3f Li(const Scene* scene,
                       Sampler* sampler,
                       const Ray3f& ray) const;
};
