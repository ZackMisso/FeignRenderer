/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel.h>

void SDFAccel::preProcess()
{
    meshes = std::vector<Shape*>();
}

void SDFAccel::clear()
{
    meshes.clear();
}

void SDFAccel::addShape(Shape* mesh)
{
    // TODO: add check to make sure shape is a sdf
    throw new NotImplementedException("sdf accel add shape");
    meshes.push_back(mesh);
}

void SDFAccel::build()
{
    throw new NotImplementedException("sdf accel build");
}

// brute force
bool SDFAccel::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    throw new NotImplementedException("sdf accel intersect");

    return false;
}
