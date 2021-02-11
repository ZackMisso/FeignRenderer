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

Trans_RatioTracking::Trans_RatioTracking()
{
    maj = 3.0;
}

Trans_RatioTracking::Trans_RatioTracking(Float maj)
    : maj(maj) { }

Color3f Trans_RatioTracking::transmittance(const Ray3f& ray,
                                           Sampler* sampler,
                                           MediaClosure& closure) const
{
    #if NONEXPMEDIA
        throw new NotSupportedException("ratio tracking does not work for non-classical media");
        return 0.f;
    #else
        Float tMin = closure.t_min;
        Float tMax = closure.t_max;

        Float t = tMin;
        Color3f tr = Color3f(1.0);

        while(true)
        {
            t -= log(1.f - sampler->next1D()) / maj;

            if (t >= tMax) break;

            Color3f ext = density->D(ray(t)) * density->sigma_t;
            ext = ext.min(maj); // for now disallow non bounding majoranst by
                                // clamping
            tr *= (Color3f(maj) - ext) / maj;
        }

        return tr;
    #endif
}

FEIGN_END()
