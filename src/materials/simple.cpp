/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/material.h>
// #include <feign/core/scene.h>

SimpleMaterial::SimpleMaterial(BSDFNode* bsdf)
    : bsdf(bsdf) { }

SimpleMaterial::~SimpleMaterial() { }

void SimpleMaterial::sample(MaterialClosure& closure) const
{
    (*bsdf)()->sample(closure);
}

void SimpleMaterial::evaluate(MaterialClosure& closure) const
{
    (*bsdf)()->evaluate(closure);
}

bool SimpleMaterial::isDelta() const
{
    return (*bsdf)()->isDelta();
}