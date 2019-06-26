#pragma once

#include <feign/emitters/emitter.h>

class PointEmitter : public Emitter
{
public:
    PointEmitter();
    PointEmitter(Node* parent);
    PointEmitter(Node* parent,
                 Color3f I,
                 Point3f pos);

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual void preProcess(bool use_prims);

    virtual std::string getName() const;

protected:
    Color3f I;
    Point3f pos;
};
