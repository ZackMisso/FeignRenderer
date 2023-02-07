/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_est.h>
#include <feign/core/closure.h>

FEIGN_BEGIN()

Color3f Trans_PseriesCMF::transmittance(const Ray3f &ray,
                                        Sampler *sampler,
                                        MediaClosure &closure) const
{
    throw new NotImplementedException("pseries_cmf");

#if NONEXPMEDIA
    throw new NotSupportedException("pseries cmf does not work for non-classical media");
    return 0.f;
#else
    // TODO
    return 0.f;
#endif
}

FEIGN_END()
