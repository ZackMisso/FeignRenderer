/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/sampler.h>
#include <feign/math/ray.h>
#include <feign/math/transform.h>
#include <feign/media/density_func.h>
#include <feign/media/phase.h>
#include <feign/media/sampling.h>
#include <feign/media/trans_est.h>

class MediaClosure;

class Media
{
public:
    Media();
    virtual ~Media();

    virtual void preProcess() { }

    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const = 0;

    virtual Color3f transmittance(Ray3f ray,
                                  Sampler* sampler,
                                  Float t_min,
                                  Float t_max) const = 0;

    virtual Float sample_phase(const Vector3f& wo,
                               Vector3f& wi,
                               Point2f samp) const = 0;

    virtual bool isGlobal() const { return false; }
};

// a generalized medium class to avoid writing all these extensions
class StandardMedium : public Media
{
public:
    struct Params
    {
    public:
        Params
        (
            std::string trans_node,
            std::string phase_node,
            std::string sampling_node,
            std::string density_func_node,
            Transform& transform,
            Color3f abs,
            Color3f scat
        ) : trans_node(trans_node),
            phase_node(phase_node),
            sampling_node(sampling_node),
            density_func_node(density_func_node),
            transform(transform),
            abs(abs),
            scat(scat) { }

        std::string trans_node;
        std::string phase_node;
        std::string sampling_node;
        std::string density_func_node;
        Transform transform;
        Color3f abs;
        Color3f scat;
    };

    StandardMedium(TransmittanceEstimatorNode* trans_node,
                   PhaseFunctionNode* phase_node,
                   MediumSamplingNode* sampling_node,
                   DensityFunctionNode* density_func_node,
                   Transform& transform,
                   Color3f abs,
                   Color3f scat);

    virtual void preProcess();

    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const;

    virtual Color3f transmittance(Ray3f ray,
                                  Sampler* sampler,
                                  Float t_min,
                                  Float t_max) const;

    virtual Float sample_phase(const Vector3f& wo,
                               Vector3f& wi,
                               Point2f samp) const
    {
        return phase->phase->sample(samp, wo, wi);
    }

    virtual bool isGlobal() const { return false; }

    DensityFunctionNode* density;
    MediumSamplingNode* sampling;
    PhaseFunctionNode* phase;
    TransmittanceEstimatorNode* trans_est;
    Transform transform;
    Color3f abs_coeff;
    Color3f sca_coeff;
    Color3f sigma_t;
};

class HomogeneousAbsorbingMedia : public Media
{
public:
    struct Params
    {
        Params(Float avg_density)
            : avg_density(avg_density) { }

        Float avg_density;
    };

    HomogeneousAbsorbingMedia(Float avg_density);

    virtual Color3f sample(Ray3f ray,
                           Sampler* sampler,
                           MediaClosure& closure) const;

    virtual Color3f transmittance(Ray3f ray,
                                  Sampler* sampler,
                                  Float t_min,
                                  Float t_max) const;

    virtual Float sample_phase(const Vector3f& wo,
                               Vector3f& wi,
                               Point2f samp) const { return 0.f; }

    virtual bool isGlobal() const;

    Float avg_density;
};

/////////////////////////////////////////////////
// Media Node structure
/////////////////////////////////////////////////
struct MediaNode : public Node
{
public:
    MediaNode() : media(nullptr) { }
    MediaNode(std::string name) : Node(name), media(nullptr) { }
    MediaNode(Media* media) : media(media) { }

    Media* operator()() { return media; }

    ~MediaNode() { delete media; }

    Media* media;
};
/////////////////////////////////////////////////
