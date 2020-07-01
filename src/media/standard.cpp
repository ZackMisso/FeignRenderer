#include <feign/media/media.h>
#include <feign/core/closure.h>

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
      sca_coeff(scat)
{
    sigma_t = abs + scat;
}

void StandardMedium::preProcess()
{
    // if (!density) assert(false);
    // assert(false);
    LOG("setting density");
    sampling->sampling->density = density->density;
    LOG("setting trans density");
    trans_est->trans_est->density = density->density;
    LOG("setting sigma_t");
    density->density->sigma_t = sigma_t;
    density->density->preProcess();
    LOG("what");
}

Color3f StandardMedium::sample(Ray3f world_ray,
                               Sampler* sampler,
                               MediaClosure& closure) const
{
    // const Ray3f ray = transform * world_ray;
    const Ray3f ray = world_ray;
    // Float min_t = ray.near;
    // Float max_t = std::min(ray.far, 20.f);

    // LOG("  min_t: " + std::to_string(min_t));
    // LOG("  max_t: " + std::to_string(max_t));

    if (sca_coeff == COLOR_BLACK)
    {
        // assert(false);
        // LOG("  transmittancing");
        // Float trans = trans_est->trans_est->transmittance(ray,
        //                                            sampler,
        //                                            min_t,
        //                                            max_t);

        // LOG("  transmittance: " + std::to_string(trans));
        // LOG("transmittance");
        // LOG("tMin: " + std::to_string(closure.t_min));
        // LOG("tMax: " + std::to_string(closure.t_max));
        return trans_est->trans_est->transmittance(ray,
                                                   sampler,
                                                   closure.t_min,
                                                   closure.t_max);
    }

    Float samp_val = sampling->sampling->sample(ray,
                                                sampler,
                                                closure.sampled_t,
                                                closure.t_min,
                                                closure.t_max);

    if (closure.handleScatter())
        return sca_coeff / (sigma_t) * samp_val;

    // TODO: this may be incorrect when non-transmittance based sampling gets
    //       implemented but for now it is assumed that if this point is reached
    //       then the point sampled has surpassed the max_t

    // assert(false);

    return Color3f(1.f);

    // // TODO: set stuff in MediumClosure
    //
    // return samp_val;
}

Color3f StandardMedium::transmittance(Ray3f world_ray,
                                      Sampler* sampler,
                                      Float t_min,
                                      Float t_max) const
{
    const Ray3f ray = transform * world_ray;

    return trans_est->trans_est->transmittance(ray,
                                               sampler,
                                               t_min,
                                               t_max);
}
