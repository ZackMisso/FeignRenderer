#include <feign/media/media.h>

StandardMedium::StandardMedium(TransmittanceEstimatorNode* trans_node,
                               PhaseFunctionNode* phase_node,
                               MediumSamplingNode* sampling_node,
                               DensityFunctionNode* density_func_node,
                               Transform& transform,
                               Color3f abs,
                               Color3f scat)
    : density(density_func_node->density),
      sampling(sampling_node->sampling),
      phase(phase_node->phase),
      trans_est(trans_node->trans_est),
      transform(transform),
      abs_coeff(abs),
      sca_coeff(scat)
{
    sampling->density = density;
    trans_est->density = density;
}

Float StandardMedium::sample(Ray3f world_ray,
                             Sampler* sampler,
                             MediaClosure& closure) const
{
    const Ray3f ray = transform * world_ray;
    Float min_t = ray.near;
    Float max_t = ray.far;

    if (sca_coeff == COLOR_BLACK)
    {
        return trans_est->transmittance(ray,
                                        sampler,
                                        min_t,
                                        max_t);
    }

    Float sample_pos;
    Float samp_val = sampling->sample(ray,
                                      sampler,
                                      sample_pos,
                                      min_t,
                                      max_t);

    // TODO: set stuff in MediumClosure

    return samp_val;
}

Float StandardMedium::transmittance(Ray3f world_ray,
                                    Sampler* sampler) const
{
    const Ray3f ray = transform * world_ray;

    return trans_est->transmittance(ray,
                                    sampler,
                                    ray.near,
                                    ray.far);
}
