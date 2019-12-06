/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>

Path_Bidirectional_Integrator::Path_Bidirectional_Integrator()
{
    throw new NotImplementedException("path bidir constructor");
}

void Path_Bidirectional_Integrator::preProcess()
{
    Integrator::preProcess();

    throw new NotImplementedException("path bidir preProcess");
}

Color3f Path_Bidirectional_Integrator::Li(const Scene* scene,
                                           Sampler* sampler,
                                           const Ray3f& ray) const
{
    throw new NotImplementedException("path bidir li");

    return Color3f(0.0);
}
