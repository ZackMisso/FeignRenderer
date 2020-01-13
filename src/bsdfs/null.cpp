/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>

NullBSDF::NullBSDF() : BSDF() { }

// the idea is that sample returns eval() / pdf()
Color3f NullBSDF::sample(BSDFQuery& rec, const Point2f& sample) const
{
    rec.wo = rec.wi;
    rec.eta = 1.0f;

    return Color3f(1.f);
}

Color3f NullBSDF::eval(const BSDFQuery& rec) const
{
    return Color3f(1.f);
}

Float NullBSDF::pdf(const BSDFQuery& rec) const
{
    return 1.f;
}
