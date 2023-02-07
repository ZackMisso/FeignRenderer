/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/math/warp.h>
#include <feign/math/coord_frame.h>

FEIGN_BEGIN()

Mirror::Mirror(Color3f albedo)
    : BSDF(), albedo(albedo) {}

void Mirror::sample(MaterialClosure &closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = Color3f(0.f);
        closure.pdf = 0.f;
    }
    else
    {
        // technically the pdf should not be defined, but it works out better
        // implementation wise if we just set it to 1
        closure.pdf = 1.f;

        closure.wo = Vector3f(
            -closure.wi(0),
            -closure.wi(1),
            closure.wi(2));

        closure.eta = 1.0f;
        closure.albedo = albedo;
    }
}

void Mirror::evaluate(MaterialClosure &closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = Color3f(0.f);
    }
    else
    {
        closure.albedo = albedo;
    }
}

FEIGN_END()
