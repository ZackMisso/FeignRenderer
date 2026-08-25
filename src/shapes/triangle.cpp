/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/shapes/triangle.h>

FEIGN_BEGIN()

Triangle::Triangle(Vec3u vs_inds,
                   Vec3u ns_inds,
                   Vec3u uvs_inds)
    : vs_inds(vs_inds), ns_inds(ns_inds), uvs_inds(uvs_inds)
{
}

Vec3u Triangle::get_vert_inds() const { return vs_inds; }
Vec3u Triangle::get_norm_inds() const { return ns_inds; }
Vec3u Triangle::get_uv_inds() const { return uvs_inds; }

FEIGN_END()
