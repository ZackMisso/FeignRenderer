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

Color3f Ray_Marching_Samp::sample(Ray3f ray,
                                  Sampler* sampler,
                                  MediaClosure& closure) const
{
    // LOG("in ray march");
    #if NONEXPMEDIA
        // LOG("nonexp");
        // TODO: implement unbiased ray-marching once paper publishes
        Float t = closure.t_min;
        Float samp = sampler->next1D();
        Color3f od = 0.0;

        // LOG("trans");
        Float od_samp = trans_func->sample(sampler,
                                           closure.last_event);
        // LOG("trans done");

        while (t < closure.t_max)
        {
            Float step = std::min(step_size, closure.t_max - t);
            Float loc = t + step * samp;

            Float past_od = od.max();
            od += density->D(ray(loc)) * density->sigma_t;

            if (od.max() > od_samp)
            {
                t = t + step * (od_samp - past_od) / (od.max() - past_od);
                closure.sampled_t = t;
                return Color3f(1.f);
            }

            t += step_size;
        }

        closure.sampled_t = closure.t_max + 1.f;

        return Color3f(1.f);
    #else
        Float t = closure.t_min;
        Float samp = sampler->next1D();
        Color3f od = 0.0;
        Float od_samp = -std::log(1.0 - sampler->next1D());

        while (t < closure.t_max)
        {
            Float step = std::min(step_size, closure.t_max - t);
            Float loc = t + step * samp;

            Float past_od = od.max();
            od += density->D(ray(loc)) * density->sigma_t;

            if (od.max() > od_samp)
            {
                t = t + step * (od_samp - past_od) / (od.max() - past_od);
                closure.sampled_t = t;
                return Color3f(1.f);
            }

            t += step_size;
        }

        closure.sampled_t = closure.t_max + 1.f;

        return Color3f(1.f);
    #endif
}

FEIGN_END()
