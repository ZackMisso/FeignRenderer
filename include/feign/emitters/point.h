/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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

    virtual void preProcess();

    virtual std::string getName() const;

protected:
    Color3f I;
    Point3f pos;
};
