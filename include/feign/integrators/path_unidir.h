/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/integrators/integrator.h>

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
