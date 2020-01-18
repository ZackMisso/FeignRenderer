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

// Color3f Mirror::sample(BSDFQuery& rec, const Point2f& sample) const
void Mirror::sample(MaterialClosure& closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = Color3f(0.f);
    }
    else
    {
        // assert(false);
        closure.wo = Vector3f(
            -closure.wi(0),
            -closure.wi(1),
             closure.wi(2)
        );

        closure.eta = 1.0f;
        closure.albedo = albedo;
    }
}

// Color3f Mirror::eval(const BSDFQuery& rec) const
void Mirror::evaluate(MaterialClosure& closure) const
{
    closure.albedo = Color3f(0.f);
}
