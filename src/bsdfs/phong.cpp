/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/bsdf.h>
#include <feign/core/sampler.h>
#include <feign/math/coord_frame.h>

Phong::Phong(Color3f kd, Color3f ks)
    : BSDF(), kd(kd), ks(ks) { }

void Phong::sample(MaterialClosure& closure) const
{
    throw new NotImplementedException("phong sample");
}

void Phong::evaluate(MaterialClosure& closure) const
{
    throw new NotImplementedException("phong eval");
}
