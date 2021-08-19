/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/media.h>
#include <feign/core/closure.h>

// TODO: this will eventually be incorporated into a generic medium
//       class once my volumetric support is fully integrated. This
//       is just a temporary hack to get absorbing media to work for
//       a specific scene

FEIGN_BEGIN()

HomogeneousAbsorbingMedia::HomogeneousAbsorbingMedia(Float avg_density)
    : avg_density(avg_density) {}

Color3f HomogeneousAbsorbingMedia::sample(Ray3f ray,
                                          Sampler *sampler,
                                          MediaClosure &closure) const
{
    closure.sampled_t = closure.t_max + 1;
    return transmittance(ray,
                         sampler,
                         closure);
}

Color3f HomogeneousAbsorbingMedia::transmittance(Ray3f ray,
                                                 Sampler *sampler,
                                                 MediaClosure &closure) const
{
    return exp(-avg_density * (closure.t_max - closure.t_min));
}

bool HomogeneousAbsorbingMedia::isGlobal() const
{
    return true;
}

FEIGN_END()
