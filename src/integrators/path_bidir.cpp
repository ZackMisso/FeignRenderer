/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>

FEIGN_BEGIN()

Path_Bidirectional_Integrator::Path_Bidirectional_Integrator(FilterNode *filter,
                                                             Integrator::Params *params)
    : Integrator(filter, params)
{
    throw new NotImplementedException("path bidir constructor");
}

void Path_Bidirectional_Integrator::preProcess(const Scene *scene, Sampler *sampler)
{
    Integrator::preProcess(scene, sampler);

    throw new NotImplementedException("path bidir preProcess");
}

Color3f Path_Bidirectional_Integrator::Li(const Scene *scene,
                                          Sampler *sampler,
                                          const Ray3f &ray,
                                          bool debug) const
{
    throw new NotImplementedException("path bidir li");

    return Color3f(0.0);
}

FEIGN_END()
