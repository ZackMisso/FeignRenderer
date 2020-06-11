#include <feign/media/media.h>

StandardMedium::StandardMedium(Params* params)
    : density(params->density_func_node->density),
      sampling(params->sampling_node->sampling),
      phase(params->phase_node->phase),
      trans_est(params->trans_node->trans_est),
      abs_coeff(params->abs),
      sca_coeff(params->scat)
{
    // TODO
    throw new NotImplementedException("standard medium");
}

Float StandardMedium::sample(Ray3f ray,
                             Sampler* sampler,
                             MediaClosure& closure) const
{
    if (sca_coeff == COLOR_BLACK)
    {
        // figure out min_t and max_t
        // only return transmittance
        // return transmittance(ray)
    }

    // TODO

    throw new NotImplementedException("sample standard medium");
    return 0.f;
}

Float StandardMedium::transmittance(Ray3f ray,
                                    Float min_t,
                                    Float max_t) const
{
    // TODO

    throw new NotImplementedException("transmittance standard medium");
    return 0.f;
}
