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

BSDF* WireframeMaterial::getBSDF(const Intersection& its) const
{
    Point3f bary = its.bary;
    float min = bary[0];
    if (bary[1] < min) min = bary[1];
    if (bary[2] < min) min = bary[2];

    if (min < threshold)
        return wireframe_bsdf;

    return mesh_bsdf;
}
