#include <feign/integrators/path_unidir.h>

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
    throw new NotImplementedException("path unidir preProcess");
}

std::string Path_Unidirectional_Integrator::getName()
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
