/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/core/scene.h>

NullBSDF::NullBSDF() : BSDF() { }

// the idea is that sample returns eval() / pdf()
// Color3f NullBSDF::sample(BSDFQuery& rec, const Point2f& sample) const
void NullBSDF::sample(MaterialClosure& closure) const
{
    closure.wo = -closure.wi;
    closure.eta = 1.0f;
    closure.albedo = Color3f(1.f);

    closure.material_accepts_shadows = false;

    // TODO: getShapeMedium is slow.. is there a better way of doing this to not
    //       slow down the rest of the renders?
    // closure.media = closure.scene->getShapeMedium(*(closure.its));
}

// Color3f NullBSDF::eval(const BSDFQuery& rec) const
void NullBSDF::evaluate(MaterialClosure& closure) const
{
    closure.albedo = Color3f(1.f);
}
