/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/math/warp.h>
#include <feign/math/coord_frame.h>

FEIGN_BEGIN()

Diffuse::Diffuse(Color3f albedo)
    : BSDF(), albedo(albedo)
{
}

void Diffuse::sample(MaterialClosure& closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0)
    {
        closure.albedo = Color3f(0.f, 0.f, 0.f);
        closure.pdf = 0.f;
    }
    else
    {
        closure.wo = WarpSpace::sqrToCosHemi(closure.sampler->next2D());
        closure.eta = 1.0f;
        closure.albedo = albedo * INV_PI;
        closure.pdf = INV_PI * CoordinateFrame::cosTheta(closure.wo);
    }
}

void Diffuse::evaluate(MaterialClosure& closure) const
{
    if (CoordinateFrame::cosTheta(closure.wi) <= 0 ||
        CoordinateFrame::cosTheta(closure.wo) <= 0)
    {
        closure.albedo = Color3f(0.f);
    }
    else
    {
        closure.albedo = albedo * INV_PI;
    }
}

FEIGN_END()
