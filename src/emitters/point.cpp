/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/emitter.h>
#include <feign/math/warp.h>

FEIGN_BEGIN()

PointEmitter::PointEmitter() : Emitter() { }

PointEmitter::PointEmitter(Color3f I,
                           Point3f pos)
    : I(I), pos(pos)
{
    // does nothing
}

void PointEmitter::preProcess() { }

Color3f PointEmitter::sample_nee(EmitterQuery& rec,
                                const Point2f& sample,
                                Float* pdf) const
{
    rec.wi = (pos - rec.p);
    rec.sqr_dist = rec.wi.sqrNorm();
    // LOG("sqr_dist", rec.sqr_dist);
    rec.wi = rec.wi.normalized();

    if (pdf) *pdf = 1.0;

    // TODO: the INV_FOURPI should actually be the pdf, and should be incorporated
    //       in later.... but whatever for now.
    return I / rec.sqr_dist * INV_FOURPI;
}

Color3f PointEmitter::sample_medium(EmitterQuery& rec,
                                    const Point2f& sample,
                                    Float* pdf) const
{
    throw new NotImplementedException("point sample pos");

    return Color3f(0.f);
}

Color3f PointEmitter::sample_ray(EmitterQuery& rec,
                                 const Point2f& dir_sample,
                                 const Point2f& point_sample,
                                 Float* pdf) const
{
    rec.p = pos; // point_sample is not used since a point light is a point
    rec.wi = WarpSpace::sqrToUniSph(dir_sample);
    *pdf = WarpSpace::sqrToUniSphPdf(rec.wi);
    return I;
}

Color3f PointEmitter::evaluate(EmitterQuery& rec) const
{
    throw new NotImplementedException("point evaluate");

    return Color3f(0.f);
}

Point3f PointEmitter::getCenter() const
{
    return pos;
}

FEIGN_END()
