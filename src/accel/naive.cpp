#include <feign/accel/naive.h>

void NaiveAccel::preProcess()
{
    preProcessChildren();

    meshes = vector<Shape*>();
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
bool NaiveAccel::intersect(const Ray3f& ray, Intersection& its)
{
    bool intersects = false;

    for (int i = 0; i < meshes.size(); ++i)
    {
        intersects |= meshes[i]->intersect(ray, its);
    }

    return intersects;
}
