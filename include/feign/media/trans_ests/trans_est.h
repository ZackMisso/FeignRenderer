#pragma once

#include <feign/common.h>
#include <feign/media/density/density_func.h>

class Transmittance_Estimator
{
public:
    Transmittance_Estimator(Density_Function* density)
        : density(density) { }

    virtual Float Tr(const Ray3f& ray,
                     Sampler* sampler,
                     Float tMin,
                     FLoat tMax) const = 0;

    virtual Color3f SpectralTr(const Ray3f& ray,
                               Sampler* sampler,
                               Float tMin,
                               Float tMax) const = 0;

protected:
    Density_Function* density;
};