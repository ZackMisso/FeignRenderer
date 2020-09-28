/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

FEIGN_BEGIN()

DistantEmitter::DistantEmitter()
{
    throw new NotImplementedException("emitter distant");
}

Color3f DistantEmitter::sample_li(EmitterQuery& rec,
                                  const Point2f& sample,
                                  Float* pdf) const
{
    // Vector3f light_pos = Warp::squareToUniformSphereCap(sample, cos_angle);
    // Vector3f light_pos_world = transform * lightPos;
    //
    // rec.wi = light_pos_world;
    // rec.sqr_dist = std::numeric_limits<float>::infinity();
    // rec.wi.normalize();
    // *pdf = Warp::squareToUniformSphereCapPdf(light_pos, cos_angle);
    //
    // return radiance;

    throw new NotImplementedException("emitter distant sample li");
}

Color3f DistantEmitter::sample_pos(EmitterQuery& rec,
                                   const Point2f& sample,
                                   Float* pdf) const
{
    throw new NotImplementedException("emitter distant sample pos");
}

Color3f DistantEmitter::sample_ray(EmitterQuery& rec,
                                   const Point2f& dir_sample,
                                   const Point2f& point_sample,
                                   Float* pdf) const
{
    throw new NotImplementedException("emitter distant sample ray");
}

void DistantEmitter::preProcess()
{
    // throw new NotImplementedException("emitter distant ");
    // does nothing for now
}

Point3f DistantEmitter::getCenter() const
{
    throw new NotImplementedException("distant get center");

    return Point3f(0.f);
}

Color3f DistantEmitter::evaluate(EmitterQuery& rec) const
{
    throw new NotImplementedException("distant evaluate");

    return Color3f(0.f);
}

FEIGN_END()
