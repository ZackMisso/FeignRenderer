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

SDFCylinder::SDFCylinder(Point3f first,
                         Point3f second,
                         float radius,
                         const MediumBoundry* boundry,
                         bool is_null)
    : SDFShape(boundry, is_null),
      first(first),
      second(second),
      radius(radius) { }

Float SDFCylinder::evaluate(Point3f pt) const
{
    // code based off of inigo quilez's implementation

    Vector3f ba = second - first;
    Vector3f pa = pt - first;
    Float baba = ba % ba;
    Float paba = pa % ba;
    float x = (pa * baba - ba * paba).norm() - radius * baba;
    float y = abs(paba - baba * 0.5) - baba * 0.5;
    float x2 = x*x;
    float y2 = y*y*baba;
    float d = (fmax(x,y)<0.0)?-fmin(x2,y2):(((x>0.0)?x2:0.0)+((y>0.0)?y2:0.0));

    return sign(d)*sqrt(abs(d))/baba;
}

BBox3f SDFCylinder::boundingBox() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    return BBox3f(first + Vector3f(radius),
                  second - Vector3f(radius));
}

// is this really needed???
Point3f SDFCylinder::centroid() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    return (first + second) / 2.0;
}

FEIGN_END()
