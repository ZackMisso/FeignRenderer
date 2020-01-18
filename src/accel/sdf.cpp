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

void SDFAccel::addShape(SDFShape* mesh)
{
    sdfs.push_back(mesh);
}

void SDFAccel::build()
{
    scene_box = BBox3f();

    for (int i = 0; i < sdfs.size(); ++i)
    {
        scene_box.expand(sdfs[i].boundingBox());
    }
}

// brute force
bool SDFAccel::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    throw new NotImplementedException("sdf accel intersect");

    return false;
}
