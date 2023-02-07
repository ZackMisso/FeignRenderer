/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>

FEIGN_BEGIN()

SpotLightEmitter::SpotLightEmitter(Point3f light_pos,
                                   Vector3f light_dir,
                                   Color3f radiance,
                                   Float light_angle)
    : light_pos(light_pos),
      light_dir(light_dir),
      radiance(radiance),
      light_angle(cos(degToRad(light_angle)))
{
    light_dir = light_dir.normalized();
}

Color3f SpotLightEmitter::sample_nee(EmitterQuery &rec,
                                     const Point2f &sample,
                                     Float *pdf) const
{
    rec.wi = (light_pos - rec.p);
    rec.wi = rec.wi.normalized();

    CoordinateFrame frame(light_dir);

    Float cos_t = frame.toLocal(-rec.wi)(2);

    // LOG("rec.wi", rec.wi);
    // LOG("light_dir", light_dir);
    // TODO: look into why the angle is flipped later

    if (pdf)
        *pdf = 1.0;

    // a%b = |a||b|cos
    if (cos_t > light_angle)
    {
        return radiance;
    }

    return COLOR_BLACK;
}

Color3f SpotLightEmitter::sample_medium(EmitterQuery &rec,
                                        const Point2f &sample,
                                        Float *pdf) const
{
    throw new NotImplementedException("spot sample pos");
}

Color3f SpotLightEmitter::sample_ray(EmitterQuery &rec,
                                     const Point2f &dir_sample,
                                     const Point2f &point_sample,
                                     Float *pdf) const
{
    throw new NotImplementedException("spot emitter sample ray");
}

Color3f SpotLightEmitter::evaluate(EmitterQuery &rec) const
{
    CoordinateFrame frame(light_dir);

    Float cos_t = frame.toLocal(-rec.wi)(2);

    if (cos_t > light_angle)
    {
        return radiance;
    }

    return COLOR_BLACK;
}

Point3f SpotLightEmitter::getCenter() const
{
    return light_pos;
}

FEIGN_END()
