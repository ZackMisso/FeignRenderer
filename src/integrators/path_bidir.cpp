/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/integrators/path_bidir.h>

Path_Bidirectional_Integrator::Path_Bidirectional_Integrator()
{
    throw new NotImplementedException("path bidir constructor");
}

Path_Bidirectional_Integrator::Path_Bidirectional_Integrator(Node* parent)
{
    throw new NotImplementedException("path bidir constructor");
}

void Path_Bidirectional_Integrator::preProcess()
{
    Integrator::preProcess();

    throw new NotImplementedException("path bidir preProcess");
}

std::string Path_Bidirectional_Integrator::getName() const
{
    throw new NotImplementedException("path bidir getName");

    return "";
}

Color3f Path_Bidirectional_Integrator::Li(const Scene* scene,
                                           Sampler* sampler,
                                           const Ray3f& ray) const
{
    throw new NotImplementedException("path bidir li");

    return Color3f(0.0);
}
