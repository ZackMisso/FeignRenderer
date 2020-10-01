/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/sdf.h>
#include <feign/math/bbox.h>

FEIGN_BEGIN()

SDFBox::SDFBox(Point3f tlc,
               Point3f brc,
               const MediumBoundry* boundry,
               bool is_null)
    : SDFShape(boundry, is_null),
      tlc(tlc),
      brc(brc) { }

Float SDFBox::evaluate(Point3f pt) const
{
    // tODO: store these instead of the current data
    Vector3f half_bounds = (tlc - brc) / 2.f;
    Vector3f center = (tlc + brc) / 2.f;
    Vector3f q = (pt - center).abs() - (half_bounds);

    return q.max(0.f).norm() + fmin(fmax(q(0),fmax(q(1),q(2))),0.f);
}

BBox3f SDFBox::boundingBox() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    return BBox3f(tlc, brc);
}

// is this really needed???
Point3f SDFBox::centroid() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    return (tlc + brc) / 2.0;
}

FEIGN_END()
