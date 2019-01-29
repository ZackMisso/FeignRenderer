#include <feign/accel/naive.h>

void NaiveAccel::preProcess()
{
    preProcessChildren();

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
            ray.maxt = its.t;
            its.intersectedMesh = meshes[i];
        }
    }

    if (intersects)
    {
        // Vec3f bary(1.0 - its.uv[0] - its.uv[1], its.uv[0], its.uv[1]);

        // const Shape* mesh = its.intersectedMesh;

        its.intersectedMesh->completeIntersectionInfo(ray, its);
    }

    // if (intersects) cout << "what" << endl;

    return intersects;
}
