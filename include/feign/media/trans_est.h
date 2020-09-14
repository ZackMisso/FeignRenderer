/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/media/density_func.h>
#include <feign/core/sampler.h>
#include <feign/math/ray.h>

FEIGN_BEGIN()

class TransmittanceEstimator
{
public:
    TransmittanceEstimator()
    {
        // density function should be initialized later i.e. in the constructor
        // of standard medium
        density = nullptr;
    }

    virtual ~TransmittanceEstimator()
    {
        density = nullptr;
    }

    TransmittanceEstimator(DensityFunction* density) : density(density) { }

    virtual Color3f transmittance(const Ray3f& ray,
                                  Sampler* sampler,
                                  Float tMin,
                                  Float tMax) const = 0;

    // virtual Color3f spectral_transmittance(const Ray3f& ray,
    //                                        Sampler* sampler,
    //                                        Float tMin,
    //                                        Float tMax) const = 0;

    DensityFunction* density;
};

class Trans_Homogenous : public TransmittanceEstimator
{
public:
    virtual Color3f transmittance(const Ray3f& ray,
                                  Sampler* sampler,
                                  Float tMin,
                                  Float tMax) const
    {
        // annalytically importance samples transmittance for a homogeneous medium
        Color3f dense = density->D(ray((tMax-tMin) / 2.f + tMin));
        Color3f trans = Exp(dense * -(tMax-tMin));
        return trans;
    }

    // virtual Color3f spectral_transmittance(const Ray3f& ray,
    //                                        Sampler* sampler,
    //                                        Float tMin,
    //                                        Float tMax) const
    // {
    //     return exp(-(tMax-tMin) *
    //            density->D(ray(((tMax-tMin) / 2.f) + tMin)));
    // }
};

class Trans_DeltaTracking : public TransmittanceEstimator
{
public:
    virtual Color3f transmittance(const Ray3f& ray,
                                  Sampler* sampler,
                                  Float tMin,
                                  Float tMax) const;

    // virtual Color3f spectral_transmittance(const Ray3f& ray,
    //                                        Sampler* sampler,
    //                                        Float tMin,
    //                                        Float tMax) const;
};

class Trans_RatioTracking : public TransmittanceEstimator
{
public:
    struct Params
    {
        Params(Float maj)
            : maj(maj) { }

        Float maj;
    };

    Trans_RatioTracking();
    Trans_RatioTracking(Float maj);

    virtual Color3f transmittance(const Ray3f& ray,
                                  Sampler* sampler,
                                  Float tMin,
                                  Float tMax) const;

    // virtual Color3f spectral_transmittance(const Ray3f& ray,
    //                                        Sampler* sampler,
    //                                        Float tMin,
    //                                        Float tMax) const;

    Float maj;
};

class Trans_PseriesCMF : public TransmittanceEstimator
{
public:
    virtual Color3f transmittance(const Ray3f& ray,
                                  Sampler* sampler,
                                  Float tMin,
                                  Float tMax) const;

    // virtual Color3f spectral_transmittance(const Ray3f& ray,
    //                                        Sampler* sampler,
    //                                        Float tMin,
    //                                        Float tMax) const;
};

/////////////////////////////////////////////////
// TransmittanceEstimator Node structure
/////////////////////////////////////////////////
struct TransmittanceEstimatorNode : public Node
{
public:
    TransmittanceEstimatorNode() : trans_est(nullptr) { }
    TransmittanceEstimatorNode(std::string name) : Node(name), trans_est(nullptr) { }
    TransmittanceEstimatorNode(TransmittanceEstimator* trans_est) : trans_est(trans_est) { }

    ~TransmittanceEstimatorNode() { delete trans_est; }

    TransmittanceEstimator* trans_est;
};
/////////////////////////////////////////////////

FEIGN_END()
