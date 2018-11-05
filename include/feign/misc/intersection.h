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

    CoordinateFrame shadingFrame;
    CoordinateFrame geometricFrame;

    const Shape* intersectedMesh;

    Intersection();

    Vector3f toLocal(const Vector3f& vec) const;
    Vector3f toWorld(const Vector3f& vec) const;
};
