#pragma once

// This implementation is based off of Nori's implementation

#include <feign/accel/accel.h>

struct BVH_Node
{
    union
    {
        struct
        {
            unsigned flag : 1;
            uint32_t size : 31;
            uint32_t start;
        } leaf;

        struct
        {
            unsigned flag : 1;
            uint32_t axis : 31;
            uint32_t rightChild;
        } inner;

        uint64_t data;
    };

    BBox3f bbox;

    bool isLeaf() const
    {
        return leaf.flag == 1;
    }

    bool isInner() const
    {
        return leaf.flag == 0;
    }

    bool empty() const
    {
        return data == 0;
    }

    uint32_t start() const
    {
        return leaf.start;
    }

    uint32_t end() const
    {
        return leaf.start + leaf.size;
    }
};

class BVH : public Accel
{
public:
    BVH();
    ~BVH();

    virtual void clear();
    virtual void build();
    virtual void addShape(Shape* mesh);

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;

    virtual std::string getName() const;

protected:
    std::vector<uint32_t> mesh_offsets;
    std::vector<BVH_Node*> nodes;
    // indices references by nodes?
    BBox3f bbox;
};
