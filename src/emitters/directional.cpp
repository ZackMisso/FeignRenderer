/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

FEIGN_BEGIN()

DirectionalEmitter::DirectionalEmitter(Vector3f light_dir, Color3f radiance)
    : light_dir(light_dir), radiance(radiance)
{
    light_dir = light_dir.normalized();
}

Color3f DirectionalEmitter::sample_nee(EmitterQuery &rec,
                                       const Point2f &sample,
                                       Float *pdf) const
{
    rec.wi = light_dir;
    if (pdf)
        *pdf = 1.0;

    // rec.sqr_dist = 1.f; // technically infinity

    return radiance;
}

Color3f DirectionalEmitter::sample_medium(EmitterQuery &rec,
                                          const Point2f &sample,
                                          Float *pdf) const
{
    throw new NotImplementedException("emitter directional");
}

Color3f DirectionalEmitter::sample_ray(EmitterQuery &rec,
                                       const Point2f &dir_sample,
                                       const Point2f &point_sample,
                                       Float *pdf) const
{
    throw new NotImplementedException("emitter directional");
}

Color3f DirectionalEmitter::evaluate(EmitterQuery &rec) const
{
    // if wi is roughly == light dir return the radiance
    if ((light_dir - rec.wi).sqrNorm() < Epsilon)
        return radiance;

    return COLOR_BLACK;
}

FEIGN_END()
