/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

 #include <feign/core/shape.h>

 SDFSphere::SDFSphere(Point3f center, Float radius)
    : center(center), radius(radius) { }

Float SDFSphere::evaluate(Point3f pt) const
{
    return (pt - center).norm() - radius;
}

BBox3f SDFSphere::boundingBox() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    Float diameter = 2.0 * radius;
    return BBox3f(center - radius,
                  diameter,
                  diameter,
                  diameter);
}

// is this really needed???
Point3f SDFSphere::centroid() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    return center;
}

void SDFSphere::completeIntersectionInfo(const Ray3f& ray,
                                         Intersection& its) const
{
    .throw new NotImplementedException("sdf sphere completeIntersectionInfo");
}
