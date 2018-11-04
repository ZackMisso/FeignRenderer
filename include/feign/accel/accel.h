#pragma once

#include <feign/node.h>
#include <feign/math/ray.h>
#include <feign/shapes/shape.h>
#include <feign/misc/intersection.h>

class Accel : public Node
{
public:
    Accel();
    ~Accel();

    virtual void clear() = 0;
    virtual void addShape(Shape* mesh) = 0;
    virtual void build() = 0;
    virtual bool intersect(const Ray3f& ray, Intersection& its) = 0;

    virtual string getName() const;
    virtual NodeType getNodeType() const;

protected:
    vector<Shape*> meshes;
};
