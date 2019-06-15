#include <feign/bsdfs/diffuse.h>
#include <feign/math/warp.h>

Diffuse::Diffuse() : BSDF() { }

Diffuse::Diffuse(Node* parent) : BSDF(parent) { }

void Diffuse::preProcess()
{
    preProcessChildren();

    primitives->findColor("albedo", albedo, Color3f(0.5));
}

// the idea is that sample returns eval() / pdf()
Color3f Diffuse::sample(BSDFQuery& rec, const Point2f& sample) const
{
    if (CoordinateFrame::cosTheta(bRec.wi) <= 0)
    {
        return Color3f(0.f, 0.f, 0.f);
    }

    rec.wo = Warp::squareToCosineHemisphere(sample);
    rec.eta = 1.0f;

    return m_albedo;
}

Color3f Diffuse::eval(const BSDFQuery& rec) const
{
    if (CoordinateFrame::cosTheta(bRec.wi) <= 0 ||
        CoordinateFrame::cosTheta(bRec.wo) <= 0)
    {
        return Color3f(0.0f);
    }

    return m_albedo * INV_PI;
}

Float Diffuse::pdf(const BSDFQuery& rec) const
{
    if (CoordinateFrame::cosTheta(bRec.wi) <= 0 ||
        CoordinateFrame::cosTheta(bRec.wo) <= 0)
    {
        return 0.0f;
    }

    return INV_PI * Frame::cosTheta(bRec.wo);
}

std::string Diffuse::getName() const
{
    return BSDF::getName() + "diffuse";
}
