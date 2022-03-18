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
#include <feign/math/ray.h>
#include <feign/math/transform.h>
#include <feign/media/density_func.h>
#include <feign/media/phase.h>
#include <feign/media/sampling.h>
#include <feign/media/trans_est.h>
#include <feign/media/trans_func.h>

FEIGN_BEGIN()

class MediaClosure;

// a generalized medium class to avoid writing all these extensions
class Media
{
public:
    struct Params
    {
    public:
        Params(
            std::string trans_node,
            std::string phase_node,
            std::string sampling_node,
            std::string density_func_node,
            std::string trans_func_node,
            Transform &transform,
            Color3f abs,
            Color3f scat) : trans_node(trans_node),
                            phase_node(phase_node),
                            sampling_node(sampling_node),
                            density_func_node(density_func_node),
                            trans_func_node(trans_func_node),
                            transform(transform),
                            abs(abs),
                            scat(scat) {}

        std::string trans_node;
        std::string phase_node;
        std::string sampling_node;
        std::string density_func_node;
        std::string trans_func_node;
        Transform transform;
        Color3f abs;
        Color3f scat;
    };

    Media(TransmittanceEstimatorNode *trans_node,
          PhaseFunctionNode *phase_node,
          MediumSamplingNode *sampling_node,
          DensityFunctionNode *density_func_node,
          TransFuncNode *trans_func_node,
          Transform &transform,
          Color3f abs,
          Color3f scat);

    void preProcess();

    Color3f sample(Ray3f ray,
                   Sampler *sampler,
                   MediaClosure &closure) const;

    Color3f transmittance(Ray3f ray,
                          Sampler *sampler,
                          MediaClosure &closure) const;

    Float sample_phase(const Vector3f &wo,
                       Vector3f &wi,
                       Point2f samp) const
    {
        return phase->phase->sample(samp, wo, wi);
    }

    bool isGlobal() const { return false; }

    DensityFunctionNode *density;
    MediumSamplingNode *sampling;
    PhaseFunctionNode *phase;
    TransmittanceEstimatorNode *trans_est;
    TransFuncNode *trans_func;

    Transform transform;
    Color3f abs_coeff;
    Color3f sca_coeff;
    Color3f sigma_t;
};

/////////////////////////////////////////////////
// Media Node structure
/////////////////////////////////////////////////
struct MediaNode : public Node
{
public:
    MediaNode() : media(nullptr) {}
    MediaNode(std::string name) : Node(name), media(nullptr) {}
    MediaNode(Media *media) : media(media) {}

    Media *operator()() { return media; }

    ~MediaNode() { delete media; }

    Media *media;
};
/////////////////////////////////////////////////

FEIGN_END()
