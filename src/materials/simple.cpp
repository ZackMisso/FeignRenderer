/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/material.h>

SimpleMaterial::SimpleMaterial(BSDFNode* bsdf)
    : bsdf(bsdf) { }

SimpleMaterial::~SimpleMaterial() { }

BSDF* SimpleMaterial::getBSDF(const Intersection& its) const
{
    return bsdf;
}
