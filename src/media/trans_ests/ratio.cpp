#include <feign/media/trans_est.h>

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
    // LOG("inside ratio tracking");
    Float t = tMin;
    Float tr = 1.0;

    while(true)
    {
        t -= log(1.f - sampler->next1D()) / maj;

        if (t >= tMax) break;

        // LOG("pre density");
        Float ext = density->D(ray(t));
        // LOG("post density");
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
