#pragma once

#include <feign/emitters/emitter.h>

class DirectionalEmitter : public Emitter
{
public:
    DirectionalEmitter();
    DirectionalEmitter(Node* parent);

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual void preProcess();

    virtual std::string getName() const;

protected:
};
