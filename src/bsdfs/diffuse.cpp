/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/math/warp.h>
#include <feign/math/coord_frame.h>

Diffuse::Diffuse() : BSDF() { }

Diffuse::Diffuse(Node* parent) : BSDF(parent) { }

Diffuse::Diffuse(Node* parent, Color3f albedo)
    : BSDF(parent), albedo(albedo)
{
    // TODO
}

void Diffuse::preProcess()
{
    preProcessChildren();
}

// the idea is that sample returns eval() / pdf()
Color3f Diffuse::sample(BSDFQuery& rec, const Point2f& sample) const
{
    if (CoordinateFrame::cosTheta(rec.wi) <= 0)
    {
        return Color3f(0.f, 0.f, 0.f);
    }

    rec.wo = WarpSpace::squareToCosineHemisphere(sample);
    rec.eta = 1.0f;

    return albedo;
}

Color3f Diffuse::eval(const BSDFQuery& rec) const
{
    if (CoordinateFrame::cosTheta(rec.wi) <= 0 ||
        CoordinateFrame::cosTheta(rec.wo) <= 0)
    {
        return Color3f(0.0f);
    }

    return albedo * INV_PI;
}

Float Diffuse::pdf(const BSDFQuery& rec) const
{
    if (CoordinateFrame::cosTheta(rec.wi) <= 0 ||
        CoordinateFrame::cosTheta(rec.wo) <= 0)
    {
        return 0.0f;
    }

    return INV_PI * CoordinateFrame::cosTheta(rec.wo);
}

std::string Diffuse::getName() const
{
    return BSDF::getName() + "diffuse";
}
