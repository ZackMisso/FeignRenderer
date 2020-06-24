#include <feign/media/media.h>

StandardMedium::StandardMedium(TransmittanceEstimatorNode* trans_node,
                               PhaseFunctionNode* phase_node,
                               MediumSamplingNode* sampling_node,
                               DensityFunctionNode* density_func_node,
                               Transform& transform,
                               Color3f abs,
                               Color3f scat)
    : density(density_func_node),
      sampling(sampling_node),
      phase(phase_node),
      trans_est(trans_node),
      transform(transform),
      abs_coeff(abs),
      sca_coeff(scat) { }

void StandardMedium::preProcess()
{
    // if (!density) assert(false);
    // assert(false);
    sampling->sampling->density = density->density;
    trans_est->trans_est->density = density->density;
}

Float StandardMedium::sample(Ray3f world_ray,
                             Sampler* sampler,
                             MediaClosure& closure) const
{
    const Ray3f ray = transform * world_ray;
    Float min_t = ray.near;
    Float max_t = std::min(ray.far, 20.f);

    // LOG("  min_t: " + std::to_string(min_t));
    // LOG("  max_t: " + std::to_string(max_t));

    if (sca_coeff == COLOR_BLACK)
    {
        // LOG("  transmittancing");
        // Float trans = trans_est->trans_est->transmittance(ray,
        //                                            sampler,
        //                                            min_t,
        //                                            max_t);

        // LOG("  transmittance: " + std::to_string(trans));
        // LOG("transmittance");
        return trans_est->trans_est->transmittance(ray,
                                                   sampler,
                                                   min_t,
                                                   max_t);
    }

    // LOG("post transmittance");

    Float sample_pos;
    Float samp_val = sampling->sampling->sample(ray,
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
    // LOG("transmittance medium");

    const Ray3f ray = transform * world_ray;

    return trans_est->trans_est->transmittance(ray,
                                               sampler,
                                               ray.near,
                                               std::min(ray.far, 20.f));
}
