#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

// TODO: maybe template this
struct Ray3f
{
    Ray3f();
    // Ray3f(Point3f o);
    Ray3f(Vector3f d);
    Ray3f(Point3f o, Vector3f d);
    Ray3f(Point3f o, Vector3f d, Float mint, Float maxt);
    Ray3f(const Ray3f& other);

    Point3f operator()(Float t) const;
    Ray3f operator~() const; // reverse ray

    Point3f origin;
    Vector3f dir;
    Float mint;
    Float maxt;
    uint32_t depth;
};
