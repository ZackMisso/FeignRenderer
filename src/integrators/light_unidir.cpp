#include <feign/integrators/light_unidir.h>

Light_Unidirectional_Integrator::Light_Unidirectional_Integrator()
{
    throw new NotImplementedException("light unidir constructor");
}

Light_Unidirectional_Integrator::Light_Unidirectional_Integrator(Node* parent)
{
    throw new NotImplementedException("light unidir constructor");
}

void Light_Unidirectional_Integrator::preProcess(bool use_prims)
{
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
