#pragma once

#include <feign/accel/accel.h>

// there is no need for this anymore
class NaiveAccel : public Accel
{
    virtual void preProcess(bool use_prims);
    virtual void clear();
    virtual void addShape(Shape* mesh);
    virtual void build();
    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;
};
