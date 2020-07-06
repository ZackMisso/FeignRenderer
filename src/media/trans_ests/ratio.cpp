/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/trans_est.h>

FEIGN_BEGIN()

Trans_RatioTracking::Trans_RatioTracking()
{
    maj = 1.0;
}

Trans_RatioTracking::Trans_RatioTracking(Float maj)
    : maj(maj) { }

Float Trans_RatioTracking::transmittance(const Ray3f& ray,
                                         Sampler* sampler,
                                         Float tMin,
                                         Float tMax) const
{
    Float t = tMin;
    Float tr = 1.0;

    while(true)
    {
        t -= log(1.f - sampler->next1D()) / maj;

        if (t >= tMax) break;

        Float ext = density->D(ray(t));
        tr *= (maj - ext) / maj;
    }

    return tr;
}

Color3f Trans_RatioTracking::spectral_transmittance(const Ray3f& ray,
                                                    Sampler* sampler,
                                                    Float tMin,
                                                    Float tMax) const
{
    throw new NotImplementedException("delta tracking spectral tr");

    // TODO

    return Color3f(0.f);
}

FEIGN_END()
