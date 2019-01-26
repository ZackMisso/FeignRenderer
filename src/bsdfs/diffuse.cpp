#include <feign/bsdfs/diffuse.h>

Diffuse::Diffuse() : BSDF() { }

Diffuse::Diffuse(Node* parent) : BSDF(parent) { }

void Diffuse::preProcess()
{
    preProcessChildren();

    primitives->findColor("albedo", albedo, Color3f(0.5));
}

Color3f Diffuse::sample(BSDFQuery& rec, const Point2f& sample) const
{
    throw new NotImplementedException("sample diffuse");
    // TODO
    return Color3f(0.0);
}

Color3f Diffuse::eval(const BSDFQuery& rec) const
{
    throw new NotImplementedException("eval diffuse");
    // TODO
    return Color3f(0.0);
}

float Diffuse::pdf(const BSDFQuery& rec) const
{
    throw new NotImplementedException("pdf diffuse");
    // TODO
    return 0.f;
}

std::string Diffuse::getName() const
{
    return BSDF::getName() + "diffuse";
}
