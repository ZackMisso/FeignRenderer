/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/shape.h>
#include <feign/math/bbox.h>

SDFCone::SDFCone(Point3f first,
                 Point3f second,
                 Float radius_1,
                 Float radius_2,
                 bool is_null)
    : SDFShape(),
      first(first),
      second(second),
      radius_1(radius_1),
      radius_2(radius_2),
      is_null(is_null) { }

Float SDFCone::evaluate(Point3f pt) const
{
    // code based off of inigo quilez's implementation

    Float rba  = radius_2 - radius_1;
    Vector3f ba = second - first;
    Vector3f pa = pt - first;
    Float baba = ba % ba;
    Float papa = pa % pa;
    Float paba = pa % ba / baba;
	Float x = sqrt(papa - paba * paba * baba);
	Float cax = fmax(0.0,x-((paba<0.5)?radius_1:radius_2));
	Float cay = abs(paba - 0.5) - 0.5;
	Float k = rba * rba + baba;
	Float f = clamp((rba * (x - radius_1) + paba * baba) / k, 0.0, 1.0 );
	Float cbx = x - radius_1 - f * rba;
	Float cby = paba - f;
	Float s = (cbx<0.0 && cay<0.0) ? -1.0 : 1.0;

	return s*sqrt(fmin(cax*cax+cay*cay*baba, cbx*cbx+cby*cby*baba));
}

BBox3f SDFCone::boundingBox() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    Float max_radius = std::max(radius_1, radius_2);

    return BBox3f(first + Vector3f(max_radius),
                  second - Vector3f(max_radius));
}

// is this really needed???
// NOTE: this is currently incorrect
Point3f SDFCone::centroid() const
{
    // TODO: how do you calculate the centroid after arbitrary deformations
    return (first + second) / 2.0;
}
