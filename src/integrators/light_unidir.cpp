/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/stats/clocker.h>

FEIGN_BEGIN()

Light_Unidirectional_Integrator::Light_Unidirectional_Integrator(FilterNode *filter,
                                                                 Integrator::Params *params)
    : Integrator(filter, params)
{
    throw new NotImplementedException("light unidir constructor");
}

void Light_Unidirectional_Integrator::preProcess(const Scene *scene,
                                                 Sampler *sampler)
{
    Integrator::preProcess(scene, sampler);

    throw new NotImplementedException("light unidir preProcess");
}

Color3f Light_Unidirectional_Integrator::Li(const Scene *scene,
                                            Sampler *sampler,
                                            const Ray3f &ray,
                                            bool debug) const
{
    throw new NotImplementedException("light unidir li");

    return Color3f(0.0);
}

FEIGN_END()
