#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>
#include <feign/math/coord_frame.h>

class Shape;

struct Intersection
{
    Point3f p;
    Point2f uv;
    float t;
    uint32_t f; // face index

    CoordinateFrame s_frame; // shading frame
    CoordinateFrame g_frame; // geometric frame

    const Shape* intersected_mesh;

    Intersection();

    Vector3f toLocal(const Vector3f& vec) const;
    Vector3f toWorld(const Vector3f& vec) const;
};
