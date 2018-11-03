#pragma once

#include <feign/filters/recon_filter.h>

class BoxFilter : public ReconstructionFilter
{
public:
    BoxFilter();
    BoxFilter(Vec2f size);

    virtual void preProcess();

    virtual float evaluate(const Point2f& p) const;

    virtual string getName() const;
};
