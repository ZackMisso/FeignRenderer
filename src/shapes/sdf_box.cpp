/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

 #include <feign/core/shape.h>

SDFBox::SDFBox(Point3f tlc, Point3f brc)
    : tlc(tlc), brc(brc) { }

Float SDFBox::evaluate(Point3f pt) const
{
    throw new NotImplementedException("sdf box evaluate");
    return 0.0;
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
