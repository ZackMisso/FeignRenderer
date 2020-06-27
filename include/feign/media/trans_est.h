/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/media/density_func.h>
#include <feign/core/sampler.h>
#include <feign/math/ray.h>

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

    virtual Float transmittance(const Ray3f& ray,
                                Sampler* sampler,
                                Float tMin,
                                Float tMax) const = 0;

    virtual Color3f spectral_transmittance(const Ray3f& ray,
                                           Sampler* sampler,
                                           Float tMin,
                                           Float tMax) const = 0;

    DensityFunction* density;
};

class Trans_Homogenous : public TransmittanceEstimator
{
public:
    virtual Float transmittance(const Ray3f& ray,
                                Sampler* sampler,
                                Float tMin,
                                Float tMax) const
    {
        Float dense = density->D(ray((tMax-tMin) / 2.f + tMin));
        Float trans = exp(-(tMax-tMin) * dense);
        // LOG(std::to_string(exp(-0)));
        // LOG("    tMin: " + std::to_string(tMin));
        // LOG("    tMax: " + std::to_string(tMax));
        // LOG("    dense: " + std::to_string(dense));
        // LOG("    trans: " + std::to_string(trans));
        // if (trans > 1.0) assert(false);
        // LOG("eval transmittance");
        return trans;
    }

    virtual Color3f spectral_transmittance(const Ray3f& ray,
                                           Sampler* sampler,
                                           Float tMin,
                                           Float tMax) const
    {
        return exp(-(tMax-tMin) *
               density->D(ray((tMax-tMin / 2.f) + tMin)));
    }
};

class Trans_DeltaTracking : public TransmittanceEstimator
{
public:
    virtual Float transmittance(const Ray3f& ray,
                                Sampler* sampler,
                                Float tMin,
                                Float tMax) const;

    virtual Color3f spectral_transmittance(const Ray3f& ray,
                                           Sampler* sampler,
                                           Float tMin,
                                           Float tMax) const;
};

class Trans_RatioTracking : public TransmittanceEstimator
{
public:
    Trans_RatioTracking();
    Trans_RatioTracking(Float maj);

    virtual Float transmittance(const Ray3f& ray,
                                Sampler* sampler,
                                Float tMin,
                                Float tMax) const;

    virtual Color3f spectral_transmittance(const Ray3f& ray,
                                           Sampler* sampler,
                                           Float tMin,
                                           Float tMax) const;

    Float maj;
};

class Trans_PseriesCMF : public TransmittanceEstimator
{
public:
    virtual Float transmittance(const Ray3f& ray,
                                Sampler* sampler,
                                Float tMin,
                                Float tMax) const;

    virtual Color3f spectral_transmittance(const Ray3f& ray,
                                           Sampler* sampler,
                                           Float tMin,
                                           Float tMax) const;
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
