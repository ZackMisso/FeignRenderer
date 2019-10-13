/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>

Path_Unidirectional_Integrator::Path_Unidirectional_Integrator()
{
    throw new NotImplementedException("path unidir constructor");
}

Path_Unidirectional_Integrator::Path_Unidirectional_Integrator(Node* parent)
{
    throw new NotImplementedException("path unidir constructor");
}

void Path_Unidirectional_Integrator::preProcess()
{
    Integrator::preProcess();

    throw new NotImplementedException("path unidir preProcess");
}

std::string Path_Unidirectional_Integrator::getName() const
{
    throw new NotImplementedException("path unidir getName");

    return "";
}

Color3f Path_Unidirectional_Integrator::Li(const Scene* scene,
                                           Sampler* sampler,
                                           const Ray3f& ray) const
{
    throw new NotImplementedException("path unidir li");

    return Color3f(0.0);
}
