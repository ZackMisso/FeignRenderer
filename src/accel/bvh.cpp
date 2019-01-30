#include <feign/accel/bvh.h>

BVH::BVH() : Accel()
{
    mesh_offsets = std::vector<uint32_t>();
    indices = std::vector<uint32_t>();
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
    indices.clear();
}

void BVH::build()
{
    uint32_t size = mesh_offsets.back();

    if (size == 0) return;

    nodes.resize(2 * size);
    memset(nodes.data(), 0, sizeof(BVH_Node) * nodes.size());
    nodes[0].bbox = bbox;
    indices.resize(size);

    if (sizeof BVH_Node != 32)
    {
        throw NotImplementedException("BVH Node is not packed");
    }

    for (uint32_t i = 0; i < size; ++i)
    {
        indices[i] = i;
    }

    uint32_t* tmp_indices = indices.data();
    uint32_t* temp = new uint32_t[size];

    throw new NotImplementedException("bvh build");
    // build task TODO

    delete[] temp;

    std::pair<float, uint32_t> stats = statistics();

    std::vector<BVH_Node> compactified(stats.second);
    std::vector<uint32_t> skipped_accum(nodes.size());

    int64_t j = nodes.size();
    int64_t skipped = 0;

    for (int64_t = stats.second-1; i >= 0; --i)
    {
        while (nodes[--j].isUnused())
        {
            skipped++;
        }

        BVH_Node& new_node = compactified[i];
        new_node = nodes[j];
        skipped_accum[j] = (uint32_t)skipped;

        if (new_node.isInner())
        {
            new_node.inner.rightChild = (uint32_t)(i + new_node.inner.rightChild -
                                                   j - (skipped - skipped_accum[new_node.inner.rightChild]));
        }
    }

    nodes = std::move(compactified);
}

std::pair<float, uint32_t> BVH::statistics(uint32_t node_idx) const
{
    const BVH_Node& node = nodes[node_idx];

    if (node.isLeaf())
    {
        return std::make_pair((float)/*BVHBuildTask::INTERSECTION_COST * */node.leaf.size, 1u);
    }
    else
    {
        std::pair<float, uint32_t> stats_left = statistics(node_idx + 1u);
        std::pair<float, uint32_t> stats_right = statistics(node.inner.rightChild);
        float saLeft = nodes[node_idx + 1u].bbox.surfaceArea();
        float saRight = nodes[node.inner.rightChild].bbox.surfaceArea();
        float saCur = node.bbox.surfaceArea();
        float sahCost = 2/* * BVHBuildTask::TRAVERSAL_COST*/ +
                        (saLeft * stats_left.first + saRight * stats_right.first) / saCur;
        return std::make_pair(sahCost, stats_left.second + stats_right.second + 1u);
    }
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
