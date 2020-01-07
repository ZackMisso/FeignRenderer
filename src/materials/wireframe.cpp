/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/material.h>

WireframeMaterial::WireframeMaterial(BSDFNode* wireframe_bsdf,
                                     BSDFNode* mesh_bsdf)
    : wireframe_bsdf(wireframe_bsdf),
      mesh_bsdf(mesh_bsdf) { }

WireframeMaterial::~WireframeMaterial() { }

// TODO: this needs to take something as a parameter
BSDF* WireframeMaterial::getBSDF(const Intersection& its) const
{
    throw new FeignRendererException("wireframe material getBSDF() not implemented");
    return nullptr;
    // TODO
}
