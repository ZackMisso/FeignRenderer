#include <feign/accel/bvh.h>

BVH::BVH() : Accel()
{
    mesh_offsets = std::vector<uint32_t>();
    nodes = std::vector<BVH_Node*>();
    mesh_offsets.push_back(0);
}

BVH::~BVH()
{
    clear();
}

void BVH::clear()
{
    for (int i = 0; i < nodes.size(); ++i)
    {
        delete nodes[i];
    }
    nodes.clear();
    mesh_offsets.clear();
    mesh_offsets.push_back(0);
}

void BVH::build()
{
    throw new NotImplementedException("bvh build");
}

void BVH::addShape(Shape* mesh)
{
    meshes.push_back(mesh);
    mesh_offsets.push_back(mesh_offsets.back() + mesh->primitiveCount());
    bbox.expand(mesh->boundingBox());
}

bool BVH::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    throw new NotImplementedException("bvh intersect");
    // TODO
    return false;
}

std::string BVH::getName() const
{
    return Accel::getName() + "bvh";
}
