/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/media/density_func.h>

class TransmittanceEstimator
{
public:
    TransmittanceEstimator(DensityFunction* density)
        : density(density) { }

    virtual Float Tr(const Ray3f& ray,
                     Sampler* sampler,
                     Float tMin,
                     Float tMax) const = 0;

    virtual Color3f SpectralTr(const Ray3f& ray,
                               Sampler* sampler,
                               Float tMin,
                               Float tMax) const = 0;

protected:
    DensityFunction* density;
};
