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

Color3f Trans_Homogenous::transmittance(const Ray3f &ray,
                                        Sampler *sampler,
                                        MediaClosure &closure) const
{
#if NONEXPMEDIA
    throw new NotImplementedException("homogenous non exp");
    return 0.f;
#else
    Float t_min = closure.t_min;
    Float t_max = closure.t_max;

    Color3f dense = density->eval(ray((t_max - t_min) / 2.f + t_min));
    Color3f trans = exponential(dense * -(t_max - t_min));
    return trans;
#endif
}

FEIGN_END()
