/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>

NullBSDF::NullBSDF() : BSDF() { }

// the idea is that sample returns eval() / pdf()
// Color3f NullBSDF::sample(BSDFQuery& rec, const Point2f& sample) const
void NullBSDF::sample(MaterialClosure& closure) const
{
    closure.wo = -closure.wi;
    closure.eta = 1.0f;
    closure.albedo = Color3f(1.f);
}

// Color3f NullBSDF::eval(const BSDFQuery& rec) const
void NullBSDF::evaluate(MaterialClosure& closure) const
{
    closure.albedo = Color3f(1.f);
}
