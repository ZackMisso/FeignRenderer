#pragma once

#include <feign/node.h>
#include <feign/math/bbox.h>
#include <feign/misc/intersection.h>

class Shape : public Node
{
public:
    Shape();
    Shape(Node* parent);
    virtual ~Shape();

    virtual void preProcess() = 0;
    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const = 0;
    virtual bool intersect(uint32_t tri,
                           const Ray3f& scene_ray,
                           Intersection& its) const = 0;

    virtual void completeIntersectionInfo(const Ray3f& ray, Intersection& its) const = 0;
    virtual uint32_t primitiveCount() const = 0;

    virtual BBox3f boundingBox() const = 0;
    virtual BBox3f boundingBox(uint32_t tri) const = 0;
    virtual Point3f centroid() const = 0;
    virtual Point3f centroid(uint32_t tri) const = 0;

    virtual std::string getName() const;

    virtual NodeType getNodeType() const;
};
