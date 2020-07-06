/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_est.h>

FEIGN_BEGIN()

Float Trans_DeltaTracking::transmittance(const Ray3f& ray,
                                         Sampler* sampler,
                                         Float tMin,
                                         Float tMax) const
{
    throw new NotImplementedException("delta tracking tr");

    // TODO

    return 0.f;
}

Color3f Trans_DeltaTracking::spectral_transmittance(const Ray3f& ray,
                                                    Sampler* sampler,
                                                    Float tMin,
                                                    Float tMax) const
{
    throw new NotImplementedException("delta tracking spectral tr");

    // TODO

    return Color3f(0.f);
}

FEIGN_END()
