#pragma once

#include <feign/integrators/integrator.h>

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
