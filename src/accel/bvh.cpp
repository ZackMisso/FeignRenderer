#include <feign/accel/bvh.h>
#include <tbb/tbb.h>
#include <atomic>

struct Bins
{
    static const int BIN_COUNT = 16;
    Bins() { memset(counts, 0, sizeof(uint32_t) * BIN_COUNT); }
    uint32_t counts[BIN_COUNT];
    BBox3f bbox[BIN_COUNT];
};

class BVHBuildTask : public tbb::task
{
private:
    BVH& bvh;
    uint32_t node_idx;
    uint32_t* start;
    uint32_t* end;
    uint32_t* temp;

public:
    /// Build-related parametrs
    enum
    {
        SERIAL_THRESHOLD = 32,
        GRAIN_SIZE = 1000,
        TRAVERSAL_COST = 1,
        INTERSECTION_COST = 1
    };

    BVHBuildTask(BVH& bvh,
                 uint32_t node_idx,
                 uint32_t* start,
                 uint32_t* end,
                 uint32_t* temp)
        : bvh(bvh),
          node_idx(node_idx),
          start(start),
          end(end),
          temp(temp) { }

    task* execute()
    {
        uint32_t size = (uint32_t) (end - start);

        BVH_Node& node = bvh.nodes[node_idx];

        if (size < SERIAL_THRESHOLD)
        {
            execute_serially(bvh, node_idx, start, end, temp);
            return nullptr;
        }

        int axis = node.bbox.majorAxis();
        float min = node.bbox.min[axis];
        float max = node.bbox.max[axis];
        float inv_bin_size = Bins::BIN_COUNT / (max - min);

        Bins bins = tbb::parallel_reduce(
            tbb::blocked_range<uint32_t>(0u, size, GRAIN_SIZE),
            Bins(),
            [&](const tbb::blocked_range<uint32_t> &range, Bins result)
            {
                for (uint32_t i = range.begin(); i != range.end(); ++i)
                {
                    uint32_t f = start[i];
                    float centroid = bvh.getCentroid(f)[axis];

                    int index = std::min(
                                    std::max((int)((centroid - min) * inv_bin_size), 0),
                                    (Bins::BIN_COUNT - 1)
                                );

                    result.counts[index]++;
                    result.bbox[index].expand(bvh.getBoundingBox(f));
                }
                return result;
            },
            [](const Bins& b1, const Bins& b2)
            {
                Bins result;
                for (int i = 0; i < Bins::BIN_COUNT; ++i)
                {
                    result.counts[i] = b1.counts[i] + b2.counts[i];
                    result.bbox[i] = BBox3f::merge(b1.bbox[i], b2.bbox[i]);
                }
                return result;
            }
        );

        BBox3f bbox_left[Bins::BIN_COUNT];
        bbox_left[0] = bins.bbox[0];

        for (int i = 1; i < Bins::BIN_COUNT; ++i)
        {
            bins.counts[i] += bins.counts[i - 1];
            bbox_left[i] = BBox3f::merge(bbox_left[i - 1], bins.bbox[i]);
        }

        BBox3f bbox_right = bins.bbox[Bins::BIN_COUNT - 1];
        BBox3f best_bbox_right;

        int64_t best_index = -1;
        float best_cost = (float) INTERSECTION_COST * size;
        float tri_factor = (float) INTERSECTION_COST / node.bbox.surfaceArea();

        for (int i = Bins::BIN_COUNT - 2; i >= 0; --i)
        {
            uint32_t prims_left = bins.counts[i];
            uint32_t prims_right = (uint32_t) (end - start) - bins.counts[i];
            float sah_cost = 2.0 * TRAVERSAL_COST + tri_factor *
                (prims_left * bbox_left[i].surfaceArea() +
                 prims_right * bbox_right.surfaceArea());

            if (sah_cost < best_cost)
            {
                best_cost = sah_cost;
                best_index = i;
                best_bbox_right = bbox_right;
            }
            bbox_right = BBox3f::merge(bbox_right, bins.bbox[i]);
        }

        if (best_index == -1)
        {
            execute_serially(bvh, node_idx, start, end, temp);
            return nullptr;
        }

        uint32_t left_count = bins.counts[best_index];
        int node_idx_left = node_idx + 1;
        int node_idx_right = node_idx + 2 * left_count;

        bvh.nodes[node_idx_left].bbox = bbox_left[best_index];
        bvh.nodes[node_idx_right].bbox = best_bbox_right;
        node.inner.axis = axis;
        node.inner.flag = 0;

        std::atomic<uint32_t> offset_left(0),
                              offset_right(bins.counts[best_index]);

        tbb::parallel_for(
            tbb::blocked_range<uint32_t>(0u, size, GRAIN_SIZE),
            [&](const tbb::blocked_range<uint32_t>& range)
            {
                uint32_t count_left = 0;
                uint32_t count_right = 0;

                for (uint32_t i = range.begin(); i != range.end(); ++i)
                {
                    uint32_t f = start[i];
                    float centroid = bvh.getCentroid(f)[axis];
                    int index = (int)((centroid - min) * inv_bin_size);
                    (index <= best_index ? count_left : count_right)++;
                }

                uint32_t idx_l = offset_left.fetch_add(count_left);
                uint32_t idx_r = offset_right.fetch_add(count_right);

                for (uint32_t i = range.begin(); i != range.end(); ++i)
                {
                    uint32_t f = start[i];
                    float centroid = bvh.getCentroid(f)[axis];
                    int index = (int)((centroid - min) * inv_bin_size);
                    if (index <= best_index)
                        temp[idx_l++] = f;
                    else
                        temp[idx_r++] = f;
                }
            }
        );

        memcpy(start, temp, size * sizeof(uint32_t));
        assert(offset_left == left_count && offset_right == size);

        tbb::task& c = *new(allocate_continuation())tbb::empty_task;
        c.set_ref_count(2);

        BVHBuildTask& b = *new(c.allocate_child())
            BVHBuildTask(bvh,
                         node_idx_right,
                         start + left_count,
                         end,
                         temp + left_count);

        spawn(b);

        recycle_as_child_of(c);

        node_idx = node_idx_left;
        end = start + left_count;

        return this;
    }

    static void execute_serially(BVH& bvh,
                                 uint32_t node_idx,
                                 uint32_t* start,
                                 uint32_t* end,
                                 uint32_t* temp)
    {
        BVH_Node& node = bvh.nodes[node_idx];
        uint32_t size = (uint32_t) (end - start);

        float best_cost = (float)INTERSECTION_COST * size;
        int64_t best_index = -1;
        int64_t best_axis = -1;
        float* left_areas = (float*)temp;

        for (int axis = 0; axis < 3; ++axis)
        {
            std::sort(start, end, [&](uint32_t f1, uint32_t f2)
            {
                return bvh.getCentroid(f1)[axis] < bvh.getCentroid(f2)[axis];
            });

            BBox3f bbox;
            for (uint32_t i = 0; i < size; ++i)
            {
                uint32_t f = *(start + i);
                bbox.expand(bvh.getBoundingBox(f));
                left_areas[i] = (float)bbox.surfaceArea();
            }

            if (axis == 0)
            {
                node.bbox = bbox;
            }

            bbox.reset();

            float tri_factor = INTERSECTION_COST / node.bbox.surfaceArea();

            for (uint32_t i = size - 1; i >= 1; --i)
            {
                uint32_t f = *(start + i);
                bbox.expand(bvh.getBoundingBox(f));

                float left_area = left_areas[i - 1];
                float right_area = bbox.surfaceArea();
                uint32_t prims_left = i;
                uint32_t prims_right = size - i;

                float sah_cost = 2.0 * TRAVERSAL_COST +
                    tri_factor * (prims_left * left_area +
                                  prims_right * right_area);

                if (sah_cost < best_cost)
                {
                    best_cost = sah_cost;
                    best_index = i;
                    best_axis = axis;
                }
            }
        }

        if (best_index == -1)
        {
            node.leaf.flag = 1;
            node.leaf.start = (uint32_t)(start - bvh.indices.data());
            node.leaf.size = size;
            return;
        }
        std::sort(start, end, [&](uint32_t f1, uint32_t f2)
        {
            return bvh.getCentroid(f1)[best_axis] < bvh.getCentroid(f2)[best_axis];
        });

        uint32_t left_count = (uint32_t)best_index;
        uint32_t node_idx_left = node_idx + 1;
        uint32_t node_idx_right = node_idx + 2 * left_count;
        node.inner.rightChild = node_idx_right;
        node.inner.axis = best_axis;
        node.inner.flag = 0;

        execute_serially(bvh,
                         node_idx_left,
                         start,
                         start + left_count,
                         temp);

        execute_serially(bvh,
                         node_idx_right,
                         start + left_count,
                         end,
                         temp + left_count);
    }
};

BVH::BVH() : Accel()
{
    mesh_offsets = std::vector<uint32_t>();
    indices = std::vector<uint32_t>();
    nodes = std::vector<BVH_Node>();
    mesh_offsets.push_back(0);
}

BVH::~BVH()
{
    clear();
}

void BVH::clear()
{
    // for (int i = 0; i < nodes.size(); ++i)
    // {
    //     delete nodes[i];
    // }

    nodes.clear();
    mesh_offsets.clear();
    mesh_offsets.push_back(0);
    indices.clear();
}

void BVH::preProcess()
{
    // does nothing
    // build();
}

void BVH::build()
{
    uint32_t size = mesh_offsets.back();

    std::cout << "Size Build: " << size << std::endl;

    if (size == 0) return;

    nodes.resize(2 * size);
    memset(nodes.data(), 0, sizeof(BVH_Node) * nodes.size());
    nodes[0].bbox = bbox;
    indices.resize(size);

    if (sizeof(BVH_Node) != 32)
    {
        throw NotImplementedException("BVH Node is not packed");
    }

    for (uint32_t i = 0; i < size; ++i)
    {
        indices[i] = i;
    }

    uint32_t* tmp_indices = indices.data();
    uint32_t* temp = new uint32_t[size];

    BVHBuildTask& task = *new(tbb::task::allocate_root())
        BVHBuildTask(*this, 0u, tmp_indices, tmp_indices + size, temp);

    tbb::task::spawn_root_and_wait(task);

    delete[] temp;

    std::pair<float, uint32_t> stats = statistics();

    std::vector<BVH_Node> compactified(stats.second);
    std::vector<uint32_t> skipped_accum(nodes.size());

    int64_t j = nodes.size();
    int64_t skipped = 0;

    for (int64_t i = stats.second-1; i >= 0; --i)
    {
        while (nodes[--j].empty())
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
    std::cout << "Adding Shape: " << mesh->primitiveCount() << std::endl;
    meshes.push_back(mesh);
    mesh_offsets.push_back(mesh_offsets.back() + mesh->primitiveCount());
    bbox.expand(mesh->boundingBox());
}

bool BVH::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    uint32_t node_idx = 0;
    uint32_t stack_idx = 0;
    uint32_t stack[64];

    Ray3f ray(scene_ray);

    its.t = std::numeric_limits<float>::infinity();

    if (ray.mint == Epsilon)
    {
        ray.mint = std::max(ray.mint, ray.mint * ray.origin[ray.origin.maxAbsIndex()]);
    }

    if (nodes.empty() || ray.maxt < ray.mint) return false;

    bool foundIntersection = false;
    uint32_t f = 0;

    while (true)
    {
        const BVH_Node& node = nodes[node_idx];

        if (!node.bbox.intersect(ray))
        {
            if (stack_idx == 0) break;
            node_idx = stack[--stack_idx];
        }

        if (node.isInner())
        {
            stack[stack_idx++] = node.inner.rightChild;
            node_idx++;
            assert(stack_idx < 64);
        }
        else
        {
            for (uint32_t i = node.start(), end = node.end(); i < end; ++i)
            {
                uint32_t idx = indices[i];
                const Shape* mesh = meshes[findMesh(idx)];

                Intersection tmp_its;

                if (mesh->intersect(idx, ray, tmp_its))
                {
                    foundIntersection = true;
                    ray.maxt = its.t = tmp_its.t;
                    its.uv = tmp_its.uv;
                    its.intersectedMesh = tmp_its.intersectedMesh;
                    f = idx;
                }
            }
            if (stack_idx == 0) break;
            node_idx = stack[--stack_idx];
            continue;
        }
    }

    if (foundIntersection)
    {
        its.intersectedMesh->completeIntersectionInfo(ray, its);
    }

    return foundIntersection;
}

std::string BVH::getName() const
{
    return Accel::getName() + "bvh";
}
