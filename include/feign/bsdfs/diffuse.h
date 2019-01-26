#pragma once

#include <feign/bsdfs/bsdf.h>

class Diffuse : public BSDF
{
public:
    Diffuse();
    Diffuse(Node* parent);

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    virtual Color3f eval(const BSDFQuery& rec) const;
    virtual float pdf(const BSDFQuery& rec) const;

    virtual void preProcess();

    virtual std::string getName() const;

protected:
    Color3f albedo;
};
