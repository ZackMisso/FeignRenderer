#pragma once

// This util was implemented based off of:
// https://github.com/tunabrain/tungsten/blob/master/src/core/renderer/TraceableScene.hpp

#include <feign/math/bbox.h>
#include <feign/math/ray.h>
#include <feign/math/vector.h>
#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>

// TODO: Namespace everything
// namespace feign
// {

namespace EmbreeUtil
{

void initDevice();
RTCDevice getDevice();

inline RTCBounds convert(const BBox3f& box)
{
    return RTCBounds{
        box.min(0), box.min(1), box.min(2),
        box.max(0), box.max(1), box.max(2)
    };
}

inline BBox3f convert(const RTCBounds& box)
{
    return BBox3f(
        Vec3f(box.lower_x, box.lower_y, box.lower_z),
        Vec3f(box.upper_x, box.upper_y, box.upper_z)
    );
}

inline Ray3f convert(const RTCRay& ray)
{
    return Ray3f(
        Point3f(ray.org_x, ray.org_y, ray.org_z),
        Vector3f(ray.dir_x, ray.dir_y, ray.dir_z)
    );
}

inline RTCRay convert(const Ray3f& ray)
{
    RTCRay new_ray;
    new_ray.org_x = ray.origin(0);
    new_ray.org_y = ray.origin(1);
    new_ray.org_z = ray.origin(2);
    new_ray.dir_x = ray.dir(0);
    new_ray.dir_y = ray.dir(1);
    new_ray.dir_z = ray.dir(2);
    new_ray.tnear = ray.near;
    new_ray.tfar = ray.far;
    return new_ray;
}

}
