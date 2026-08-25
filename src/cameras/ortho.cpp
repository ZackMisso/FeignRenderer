/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/camera.h>

FEIGN_BEGIN()

Orthographic::Orthographic() : Camera()
{
    // TODO
}

void Orthographic::pre_process()
{
    throw new NotImplementedException("pre_process ortho");
}

Color3f Orthographic::sample_ray(Ray3f &ray,
                                 const Point2f &film_samp,
                                 const Point2f &app_samp) const
{
    throw new NotImplementedException("ortho sample_ray");

    return Color3f(1.0);
}

FEIGN_END()
