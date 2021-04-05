/**
* Author:    Zackary Misso
* Version:   0.2.0
*
* Anyone has permission to use the following code as long as proper
* acknowledgement is provided to the original author(s).
**/

#include <feign/core/emitter.h>

FEIGN_BEGIN()

EnvironmentEmitter::EnvironmentEmitter(TextureNode* texture, Color3f intensity)
    : texture(texture), intensity(intensity) { }

Color3f EnvironmentEmitter::sample_nee(EmitterQuery& rec,
                                       const Point2f& sample,
                                       Float* pdf) const
{
    // TODO: this should not be called unless importance sampling gets implemented
    throw new NotImplementedException("emitter environment");
}

Color3f EnvironmentEmitter::sample_medium(EmitterQuery& rec,
                                          const Point2f& sample,
                                          Float* pdf) const
{
    // TODO: this should not be called unless importance sampling gets implemented
    throw new NotImplementedException("emitter environment");
}

Color3f EnvironmentEmitter::sample_ray(EmitterQuery& rec,
                                       const Point2f& dir_sample,
                                       const Point2f& point_sample,
                                       Float* pdf) const
{
    // TODO: this should not be called unless importance sampling gets implemented
    throw new NotImplementedException("emitter environment sample ray");
}

void EnvironmentEmitter::preProcess()
{
    // TODO: this is where we can intiialize the pmf and cmf for importance
    //       sampling the texture if the texture allows for it
}

// TODO: support arbitrary rotations of the environment map
Color3f EnvironmentEmitter::evaluate(EmitterQuery& rec) const
{
    // convert the direction into spherical coordinates
    Float phi = std::atan2(rec.wi(2), rec.wi(0));
    if (phi < 0.f) phi = phi + 2.f * M_PI;

    double theta = acos(rec.wi(1)); // this assumes that the direction is normalized

    return texture->texture->evaluate(Point2f(phi * INV_TWOPI, theta * INV_PI));
}

FEIGN_END()
