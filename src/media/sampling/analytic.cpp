/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/sampling.h>
#include <feign/core/closure.h>
#include <feign/core/sampler.h>

FEIGN_BEGIN()

Color3f AnalyticalTrans_Samp::sample(Ray3f ray,
                                     Sampler* sampler,
                                     MediaClosure& closure) const
{
    #if NONEXPMEDIA
        throw new NotImplementedException("analytic samp non-exp");
        return 0.f;
    #else
        Float transmittance = sampler->next1D();
        Float goal_dens = -log(transmittance);

        Float dense = (density->D(ray((closure.t_max - closure.t_min) / 2.f + closure.t_min)) * density->sigma_t).max();
        Float dist = goal_dens / dense;
        closure.sampled_t = closure.t_min + dist;

        return Color3f(1.f);
    #endif
}

FEIGN_END()
