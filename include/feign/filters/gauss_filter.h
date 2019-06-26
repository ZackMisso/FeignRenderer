#pragma once

#include <feign/filters/recon_filter.h>

class GaussFilter : public ReconstructionFilter
{
public:
    GaussFilter();
    GaussFilter(Vec2f size);

    virtual void preProcess(bool use_prims);

    virtual float evaluate(const Point2f& p) const;

    virtual std::string getName() const;

protected:
    Vec2f expCoeff;
    float alpha;
};
