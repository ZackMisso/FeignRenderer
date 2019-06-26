#pragma once

#include <feign/bsdfs/bsdf.h>

class Diffuse : public BSDF
{
public:
    Diffuse();
    Diffuse(Node* parent);
    Diffuse(Node* parent, Color3f albedo);

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    virtual Color3f eval(const BSDFQuery& rec) const;
    virtual Float pdf(const BSDFQuery& rec) const;

    virtual bool isDelta() const { return false; }

    virtual void preProcess(bool use_prims);

    virtual std::string getName() const;

protected:
    Color3f albedo;
};
