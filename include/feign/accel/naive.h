#pragma once

#include <feign/accel/accel.h>

class NaiveAccel : public Accel
{
    virtual void preProcess();
    virtual void clear();
    virtual void addShape(Shape* mesh);
    virtual void build();
    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;
};