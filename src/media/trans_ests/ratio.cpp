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

Color3f Trans_RatioTracking::transmittance(const Ray3f& ray,
                                           Sampler* sampler,
                                           Float tMin,
                                           Float tMax) const
{
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
}

// Color3f Trans_RatioTracking::spectral_transmittance(const Ray3f& ray,
//                                                     Sampler* sampler,
//                                                     Float tMin,
//                                                     Float tMax) const
// {
//     throw new NotImplementedException("delta tracking spectral tr");
//
//     // TODO
//
//     return Color3f(0.f);
// }

FEIGN_END()
