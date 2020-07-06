/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/density_func.h>

FEIGN_BEGIN()

PointAverageDensity::PointAverageDensity()
{
    throw new NotImplementedException("point average density");
    // TODO
}

Float PointAverageDensity::D(const Point3f& p) const
{
    throw new NotImplementedException("point average density");
    return 0.f;
}

Color3f PointAverageDensity::SpectralD(const Point3f& p) const
{
    throw new NotImplementedException("point average density");
    return Color3f(0.f);
}

Float PointAverageDensity::maxDensity() const
{
    throw new NotImplementedException("point average density");
    return 0.f;
}

FEIGN_END()
