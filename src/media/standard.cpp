/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/media.h>
#include <feign/core/closure.h>

FEIGN_BEGIN()

Media::Media(TransmittanceEstimatorNode *trans_node,
             PhaseFunctionNode *phase_node,
             MediumSamplingNode *sampling_node,
             DensityFunctionNode *density_func_node,
             TransFuncNode *trans_func_node,
             Transform &transform,
             Color3f abs,
             Color3f scat)
    : density(density_func_node),
      trans_func(trans_func_node),
      sampling(sampling_node),
      phase(phase_node),
      trans_est(trans_node),
      transform(transform),
      abs_coeff(abs),
      sca_coeff(scat)
{
    sigma_t = abs + scat;
}

void Media::preProcess()
{
    sampling->sampling->density = density->density;
    sampling->sampling->trans_func = trans_func->trans_func;
    trans_est->trans_est->density = density->density;
    trans_est->trans_est->trans_func = trans_func->trans_func;
    density->density->sigma_t = sigma_t;
    density->density->preProcess();
}

Color3f Media::sample(Ray3f world_ray,
                      Sampler *sampler,
                      MediaClosure &closure) const
{
    const Ray3f ray = transform.inverse() * world_ray;

    // if the medium is fully absorptive, just evaluate transmittance
    if (sca_coeff == COLOR_BLACK)
    {
        return trans_est->trans_est->transmittance(ray,
                                                   sampler,
                                                   closure);
    }

    Color3f samp_val = sampling->sampling->sample(ray,
                                                  sampler,
                                                  closure);

    if (closure.handleScatter())
        return sca_coeff / (sigma_t)*samp_val;

    // TODO: this may be incorrect when non-transmittance based sampling gets
    //       implemented but for now it is assumed that if this point is reached
    //       then the point sampled has surpassed the max_t

    return Color3f(1.f);
}

Color3f Media::transmittance(Ray3f world_ray,
                             Sampler *sampler,
                             MediaClosure &closure) const
{
    const Ray3f ray = transform.inverse() * world_ray;

    return trans_est->trans_est->transmittance(ray,
                                               sampler,
                                               closure);
}

FEIGN_END()
