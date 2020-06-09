/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

// should this have access to the media closure???

class MediumSampling
{
public:
    // virtual Float sample(Ray3f ray,
    //                      Sampler* sampler,
    //                      MediaClosure& closure) const = 0;
};

class Delta_Tracking : public MediumSampling
{
public:
    Delta_Tracking();

    // virtual Float sample(Ray3f ray,
    //                      Sampler* sampler,
    //                      MediaClosure& closure) const
    // {
    //     throw new NotImplementedException("delta tracking sampler");
    //     // TODO
    //     return 0.f;
    // }

    DensityFunction* density_ref;
};

class Equidistant_Sampling : public MediumSampling
{
    Equidistant_Sampling();

    // virtual Float sample(Ray3f ray,
    //                      Sampler* sampler,
    //                      MediaClosure& closure) const
    // {
    //     throw new NotImplementedException("equi-distant sampler");
    //     // TODO
    //     return 0.f;
    // }
};
