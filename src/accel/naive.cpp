/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel.h>

void NaiveAccel::preProcess()
{
    // preProcessChildren();

    meshes = std::vector<Shape*>();
}

void NaiveAccel::clear()
{
    meshes.clear();
}

void NaiveAccel::addShape(Shape* mesh)
{
    meshes.push_back(mesh);
}

void NaiveAccel::build() { }

// brute force
bool NaiveAccel::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    Ray3f ray(scene_ray);

    bool intersects = false;

    for (int i = 0; i < meshes.size(); ++i)
    {
        if (meshes[i]->intersect(ray, its))
        {
            intersects = true;
            ray.far = its.t;
            its.intersected_mesh = meshes[i];
        }
    }

    if (intersects)
    {
        its.intersected_mesh->completeIntersectionInfo(ray, its);
    }

    return intersects;
}
