#include <feign/media/trans_est.h>

Float Trans_RatioTracking::transmittance(const Ray3f& ray,
                                         Sampler* sampler,
                                         Float tMin,
                                         Float tMax) const
{
    throw new NotImplementedException("delta tracking tr");

    // TODO

    return 0.f;
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
