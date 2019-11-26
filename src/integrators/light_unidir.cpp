/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>

Light_Unidirectional_Integrator::Light_Unidirectional_Integrator()
{
    throw new NotImplementedException("light unidir constructor");
}

void Light_Unidirectional_Integrator::preProcess()
{
    Integrator::preProcess();

    throw new NotImplementedException("light unidir preProcess");
}

std::string Light_Unidirectional_Integrator::getName() const
{
    throw new NotImplementedException("light unidir getName");

    return "";
}

Color3f Light_Unidirectional_Integrator::Li(const Scene* scene,
                                           Sampler* sampler,
                                           const Ray3f& ray) const
{
    throw new NotImplementedException("light unidir li");

    return Color3f(0.0);
}
