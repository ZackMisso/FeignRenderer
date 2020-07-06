/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/texture.h>

FEIGN_BEGIN()

SinTexture::SinTexture(Vec3f amp,
                       Vec3f phase,
                       Vec3f freq,
                       Vec3f y)
    : amp(amp), phase(phase), y(y), freq(freq) { }

void SinTexture::preProcess() { }

Color3f SinTexture::evaluate(const Point2f& point)
{
    Point2f center = Point2f(0.5, 0.5);
    Float dist = (center - point).norm();
    return Color3f(
        amp(0) * sin(dist * freq(0) + phase(0)) + y(0),
        amp(1) * sin(point(1) * freq(1) + phase(1)) + y(1),
        amp(2) * sin(phase(2) * freq(2)) + y(2)
    );
}

FEIGN_END()
