/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

// TODO: maybe create a separate abstraction for acceleration
class Triangle
{
public:
    Triangle(Vec3u vs_inds,
             Vec3u ns_inds,
             Vec3u uvs_inds);

    Vec3u get_vert_inds() const;
    Vec3u get_norm_inds() const;
    Vec3u get_uv_inds() const;

    Vec3u vs_inds;
    Vec3u ns_inds;
    Vec3u uvs_inds;
};

FEIGN_END()
