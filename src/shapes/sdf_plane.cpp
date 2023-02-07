/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/sdf.h>

FEIGN_BEGIN()

SDFPlane::SDFPlane(Point3f center,
                   Normal3f normal,
                   Float inter,
                   const MediumBoundry *boundry,
                   bool is_null)
    : SDFShape(boundry, is_null),
      center(center),
      normal(normal)
{
    interp = inter;
}

Float SDFPlane::evaluate(Point3f pt) const
{
    // a plane does not have an "inside", so the sdf should always be positive
    return std::abs((pt - center) % normal);
}

BBox3f SDFPlane::boundingBox() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    // Float diameter = 2.0 * radius;
    // return BBox3f(center - radius,
    //               diameter,
    //               diameter,
    //               diameter);

    return BBox3f(center - Vec3f(5000.f, 5000.f, 5000.f),
                  10000.f,
                  10000.f,
                  10000.f);
}

// is this really needed???
Point3f SDFPlane::centroid() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    return center;
}

FEIGN_END()
