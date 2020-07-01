/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

// should this have access to the media closure???

class MediumSampling
{
public:
    virtual ~MediumSampling() { }
    virtual Float sample(Ray3f ray,
                         Sampler* sampler,
                         Float& t,
                         Float min_t,
                         Float max_t) const = 0;

    DensityFunction* density;
};

// TODO: should ray marching be its own medium implementation???
class Ray_Marching_Samp : public MediumSampling
{
    Ray_Marching_Samp() { }

    virtual Float sample(Ray3f ray,
                         Sampler* sampler,
                         Float& t,
                         Float min_t,
                         Float max_t) const = 0;
};

// this assumes the medium is fully homogeneous
class AnalyticalTrans_Samp : public MediumSampling
{
public:
    AnalyticalTrans_Samp() { }

    virtual Float sample(Ray3f ray,
                         Sampler* sampler,
                         Float& t,
                         Float min_t,
                         Float max_t) const
    {
        Float transmittance = sampler->next1D();
        Float goal_dens = -log(transmittance);

        Float dense = density->D(ray((max_t - min_t) / 2.f + min_t));
        Float dist = goal_dens / dense;
        t = min_t + dist;

        return 1.f;
    }
};

class Delta_Tracking : public MediumSampling
{
public:
    Delta_Tracking(Float majorant)
        : majorant(majorant) { }

    virtual Float sample(Ray3f ray,
                         Sampler* sampler,
                         Float& t,
                         Float min_t,
                         Float max_t) const
    {
        // Run delta-tracking iterations to sample a medium interaction
        // Float invMaxDensity = density->maxDensity();
        // t = min_t;
        //
        // while (true)
        // {
        //     t -= std::log(1 - sampler.Get1D()) * invMaxDensity / sigma_t;
        //     if (t >= tMax) break;
        //
        //     Float dense = density->D(ray(t));
        //
        //     if (dense * invMaxDensity > sampler.Get1D()) {
        //         // Populate _mi_ with medium interaction information and return
        //         return 1.f;
        //     }
        // }

        return 1.f;
    }

    Float majorant;
};

class Equidistant_Sampling : public MediumSampling
{
public:
    Equidistant_Sampling() { }

    virtual Float sample(Ray3f ray,
                         Sampler* sampler,
                         Float& t,
                         Float min_t,
                         Float max_t) const
    {
        throw new NotImplementedException("equi-distant sampler");
        // TODO
        return 0.f;
    }
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
