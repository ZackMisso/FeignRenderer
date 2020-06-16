/**
 * Author:    Zackary Misso
 * Version:   0.1.1
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

HomogeneousAbsorbingMedia::HomogeneousAbsorbingMedia(Float avg_density)
    : avg_density(avg_density) { }

Float HomogeneousAbsorbingMedia::sample(Ray3f ray,
                                        Sampler* sampler,
                                        MediaClosure& closure) const
{
    closure.sampled_t = closure.max_t + 1;
    return transmittance(ray, sampler);
}

Float HomogeneousAbsorbingMedia::transmittance(Ray3f ray,
                                               Sampler* sampler) const
{
    return exp(-avg_density * (ray.near - ray.far));
}

bool HomogeneousAbsorbingMedia::isGlobal() const
{
    return true;
}
