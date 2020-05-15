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
//       class once my volumetric support is fully integrated

HomogeneousAbsorbingMedia::HomogeneousAbsorbingMedia(Float avg_density)
    : avg_density(avg_density)
{

}

Float HomogeneousAbsorbingMedia::sample(Ray3f ray,
                                        Sampler* sampler,
                                        MediaClosure& closure) const
{
    closure.sampled_t = closure.max_t + 1;
    return transmittance(ray, ray.near, closure.max_t);
}

Float HomogeneousAbsorbingMedia::transmittance(Ray3f ray,
                                               Float min_t,
                                               Float max_t) const
{
    return exp(-avg_density * (max_t - min_t));
}

bool HomogeneousAbsorbingMedia::isGlobal() const
{
    return true;
}
