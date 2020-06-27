/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>
#include <feign/math/coord_frame.h>
#include <feign/media/media.h>

class Shape;
class Media;

// TODO: add an intersection data struct for applying shaders
struct Intersection
{
    Point3f p;
    Point2f uv;
    float t;
    uint32_t f; // face index

    // used for shaders
    Point3f bary;

    CoordinateFrame s_frame; // shading frame
    CoordinateFrame g_frame; // geometric frame

    const Shape* intersected_mesh;

    Intersection();
    Intersection(const Intersection& its);

    Vector3f toLocal(const Vector3f& vec) const;
    Vector3f toWorld(const Vector3f& vec) const;

    Vector3f toLocalGeom(const Vector3f& vec) const;
    Vector3f toWorldGeom(const Vector3f& vec) const;
};
