/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

// TODO: maybe template this
struct Ray3f
{
    Ray3f();
    Ray3f(Vector3f d);
    Ray3f(Point3f o, Vector3f d);
    Ray3f(Point3f o, Vector3f d, Float n, Float f);
    Ray3f(const Ray3f& other);

    Point3f operator()(Float t) const;
    Ray3f operator~() const; // reverse ray

    Point3f origin;
    Vector3f dir;
    Float near;
    Float far;
    uint32_t depth;
};
