/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/sampling.h>
#include <feign/core/closure.h>
#include <feign/core/sampler.h>

FEIGN_BEGIN()

Color3f Delta_Tracking::sample(Ray3f ray,
                               Sampler *sampler,
                               MediaClosure &closure) const
{

#if NONEXPMEDIA
    throw new NotSupportedException("delta sampling non-exp");
    return 0.f;
#else
    // Run delta-tracking iterations to sample a medium interaction
    Float t = closure.t_min;

    while (true)
    {
        t -= std::log(1 - sampler->next1D()) / majorant;
        if (t >= closure.t_max)
            break;

        Float dense = (density->D(ray(t)) * density->sigma_t).max();

        if (dense / majorant > sampler->next1D())
        {
            // t should be set already
            closure.sampled_t = t;
            return Color3f(1.f);
        }
    }
    closure.sampled_t = t;

    return Color3f(1.f);
#endif
}

FEIGN_END()
