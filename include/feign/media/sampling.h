/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/sampler.h>
#include <feign/media/density_func.h>
#include <feign/media/trans_func.h>
#include <feign/math/ray.h>

FEIGN_BEGIN()

class MediaClosure;

class MediumSampling
{
public:
    virtual ~MediumSampling() { }
    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const = 0;

    DensityFunction* density;
    TransFunc* trans_func;
};

// this assumes the medium is fully homogeneous
class AnalyticalTrans_Samp : public MediumSampling
{
public:
    AnalyticalTrans_Samp() { }

    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const;
};

class Delta_Tracking : public MediumSampling
{
public:
    Delta_Tracking(Float majorant)
        : majorant(majorant) { }

    // TODO: this does not support spectral media
    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const;

    Float majorant;
};

// TODO: change this name to be consistent with its transmittance estimator
class Ray_Marching_Samp : public MediumSampling
{
public:
    struct Params
    {
        Params(Float step_size)
            : step_size(step_size) { }

        Float step_size;
    };

    Ray_Marching_Samp() : step_size(1.f) { }
    Ray_Marching_Samp(Float step_size) : step_size(step_size) { }

    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const;

    Float step_size;
};

/////////////////////////////////////////////////
// MediumSampling Node structure
/////////////////////////////////////////////////
struct MediumSamplingNode : public Node
{
public:
    MediumSamplingNode() : sampling(nullptr) { }
    MediumSamplingNode(std::string name) : Node(name), sampling(nullptr) { }
    MediumSamplingNode(MediumSampling* sampling) : sampling(sampling) { }

    ~MediumSamplingNode() { delete sampling; }

    MediumSampling* sampling;
};
/////////////////////////////////////////////////

FEIGN_END()
