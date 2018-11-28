#pragma once

#include <feign/node.h>
#include <feign/misc/intersection.h>

class Shape : public Node
{
public:
    Shape();
    Shape(Node* parent);
    virtual ~Shape();

    virtual void preProcess() = 0;
    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) = 0;
    virtual void completeIntersectionInfo(const Ray3f& ray, Intersection& its) const = 0;

    virtual string getName() const;

    virtual NodeType getNodeType() const;
};
