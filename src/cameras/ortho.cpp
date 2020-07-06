/**
 * Author:    Zackary Misso
 * Version:   0.2.0
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

void Orthographic::preProcess()
{
    throw new NotImplementedException("preprocess ortho");
}

Color3f Orthographic::sampleRay(Ray3f& ray,
                                const Point2f& filmSamp,
                                const Point2f& appSamp) const
{
    throw new NotImplementedException("ortho sampleRay");

    return Color3f(1.0);
}

FEIGN_END()
