/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/math/warp.h>
#include <feign/math/coord_frame.h>

Mirror::Mirror(Color3f albedo)
    : BSDF(), albedo(albedo) { }

Color3f Mirror::sample(BSDFQuery& rec, const Point2f& sample) const
{
    if (CoordinateFrame::cosTheta(rec.wi) <= 0)
    {
        return Color3f(0.f, 0.f, 0.f);
    }

    rec.wo = Vector3f(
        -rec.wi(0),
        -rec.wi(1),
         rec.wi(2)
    );
    rec.eta = 1.0f;

    return albedo;
}

Color3f Mirror::eval(const BSDFQuery& rec) const
{
    return Color3f(0.f);
}
