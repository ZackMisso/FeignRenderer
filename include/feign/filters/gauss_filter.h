#pragma once

#include <feign/filters/recon_filter.h>

class GaussFilter : public ReconstructionFilter
{
public:
    GaussFilter();
    GaussFilter(Vec2f size);

    virtual void preProcess();

    virtual float evaluate(const Point2f p) const;

protected:
    Vec2f expCoeff;
    float alpha;
};
