/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_est.h>
#include <feign/core/closure.h>

FEIGN_BEGIN()

Color3f Trans_RayMarching::transmittance(const Ray3f &ray,
                                         Sampler *sampler,
                                         MediaClosure &closure) const
{
#if NONEXPMEDIA
    // TODO: when I implement unbiased ray-marching in the future, it will be
    //       necessary to specify what the next-event is, however for now, none
    //       of the transmittance evaluations should ever end in a media event.

    Float min_t = closure.t_min;
    Float max_t = closure.t_max;
    Float t = min_t;
    Float samp = sampler->next1D();
    Color3f od = 0.0;

    while (t < max_t)
    {
        Float step = std::min(step_size, max_t - t);
        Float loc = t + step * samp;

        od += density->D(ray(loc)) * density->sigma_t;

        t += step_size;
    }

    return trans_func->eval(od,
                            closure.last_event,
                            closure.next_event);
#else
    Float min_t = closure.t_min;
    Float max_t = closure.t_max;
    Float t = min_t;
    Float samp = sampler->next1D();
    Color3f od = 0.0;

    while (t < max_t)
    {
        Float step = std::min(step_size, max_t - t);
        Float loc = t + step * samp;

        od += density->D(ray(loc)) * density->sigma_t;

        t += step_size;
    }

    return Exp(-od);
#endif
}

FEIGN_END()
