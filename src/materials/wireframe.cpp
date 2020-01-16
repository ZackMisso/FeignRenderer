// /**
//  * Author:    Zackary Misso
//  * Version:   0.1.1
//  *
//  * Anyone has permission to use the following code as long as proper
//  * acknowledgement is provided to the original author(s).
//  **/
//
// #include <feign/core/material.h>
//
// WireframeMaterial::WireframeMaterial(BSDFNode* wireframe_bsdf,
//                                      BSDFNode* mesh_bsdf,
//                                      Float threshold)
//     : wireframe_bsdf(wireframe_bsdf),
//       mesh_bsdf(mesh_bsdf),
//       threshold(threshold) { }
//
// WireframeMaterial::~WireframeMaterial() { }
//
// BSDF* WireframeMaterial::getBSDF(const Intersection& its,
//                                  Color3f& mat_scale) const
// {
//     Point3f bary = its.bary;
//     float min = bary[0];
//     if (bary[1] < min) min = bary[1];
//     if (bary[2] < min) min = bary[2];
//
//     mat_scale = Color3f(1.f);
//
//     if (min < threshold)
//         return wireframe_bsdf->bsdf;
//
//     return mesh_bsdf->bsdf;
// }
