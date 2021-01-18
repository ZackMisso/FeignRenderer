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
#include <feign/math/ray.h>

FEIGN_BEGIN()

class MediaClosure;

// TODO: replace parameters with closure reference

// class MediumSampling
// {
// public:
//     virtual ~MediumSampling() { }
//     virtual Color3f sample(Ray3f ray,
//                            Sampler* sampler,
//                            Float& t,
//                            Float min_t,
//                            Float max_t) const = 0;
//
//     DensityFunction* density;
// };

class MediumSampling
{
public:
    virtual ~MediumSampling() { }
    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const = 0;

    DensityFunction* density;
};

// // this assumes the medium is fully homogeneous
// class AnalyticalTrans_Samp : public MediumSampling
// {
// public:
//     AnalyticalTrans_Samp() { }
//
//     virtual Color3f sample(Ray3f ray,
//                            Sampler* sampler,
//                            Float& t,
//                            Float min_t,
//                            Float max_t) const
//     {
//         Float transmittance = sampler->next1D();
//         Float goal_dens = -log(transmittance);
//
//         Float dense = (density->D(ray((max_t - min_t) / 2.f + min_t)) * density->sigma_t).max();
//         Float dist = goal_dens / dense;
//         t = min_t + dist;
//
//         return Color3f(1.f);
//     }
// };

// this assumes the medium is fully homogeneous
class AnalyticalTrans_Samp : public MediumSampling
{
public:
    AnalyticalTrans_Samp() { }

    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const;
};

// class Delta_Tracking : public MediumSampling
// {
// public:
//     Delta_Tracking(Float majorant)
//         : majorant(majorant) { }
//
//     // TODO: this does not support spectral media
//     virtual Color3f sample(Ray3f ray,
//                            Sampler* sampler,
//                            Float& t,
//                            Float min_t,
//                            Float max_t) const
//     {
//         // Run delta-tracking iterations to sample a medium interaction
//         t = min_t;
//
//         while (true)
//         {
//             t -= std::log(1 - sampler->next1D()) / majorant;
//             if (t >= max_t) break;
//
//             Float dense = (density->D(ray(t)) * density->sigma_t).max();
//
//             if (dense / majorant > sampler->next1D())
//             {
//                 // t should be set already
//                 return Color3f(1.f);
//             }
//         }
//
//         return Color3f(1.f);
//     }
//
//     Float majorant;
// };

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

// // TODO: should ray marching be its own medium implementation???
// class Ray_Marching_Samp : public MediumSampling
// {
//     Ray_Marching_Samp() : step_size(1.f) { }
//     Ray_Marching_Samp(Float step_size) : step_size(step_size) { }
//
//     virtual Color3f sample(Ray3f ray,
//                            Sampler* sampler,
//                            Float& t,
//                            Float min_t,
//                            Float max_t) const
//     {
//         t = min_t;
//         Float samp = sampler->next1D();
//         Color3f od = 0.0;
//         Float od_samp = -std::log(1.0 - sampler->next1D());
//
//         while (t < max_t)
//         {
//             Float step = std::min(step_size, max_t - t);
//             Float loc = t + step * samp;
//
//             Float past_od = od.max();
//             od += density->D(ray(loc)) * density->sigma_t;
//
//             if (od.max() > od_samp)
//             {
//                 t = t + step * (od_samp - past_od) / (od.max() - past_od);
//                 return Color3f(1.f);
//             }
//
//             t += step_size;
//         }
//
//         return Color3f(1.f);
//     }
//
//     Float step_size;
// };

// TODO: should ray marching be its own medium implementation???
class Ray_Marching_Samp : public MediumSampling
{
    Ray_Marching_Samp() : step_size(1.f) { }
    Ray_Marching_Samp(Float step_size) : step_size(step_size) { }

    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const;

    Float step_size;
};

// class Equidistant_Sampling : public MediumSampling
// {
// public:
//     Equidistant_Sampling() { }
//
//     virtual Color3f sample(Ray3f ray,
//                            Sampler* sampler,
//                            Float& t,
//                            Float min_t,
//                            Float max_t) const
//     {
//         throw new NotImplementedException("equi-distant sampler");
//         // TODO
//         return Color3f(0.f);
//     }
// };

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
