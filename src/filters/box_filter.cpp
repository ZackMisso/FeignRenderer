/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/recon_filter.h>

FEIGN_BEGIN()

BoxFilter::BoxFilter() : ReconstructionFilter()
{
    size = Vec2f(0.5, 0.5);
}

BoxFilter::BoxFilter(Vec2f size) : ReconstructionFilter(size) {}

void BoxFilter::preProcess()
{
    // does nothing
}

Float BoxFilter::evaluate(const Point2f &p) const
{
    if (std::abs(p(0)) < size(0) && std::abs(p(1)) < size(1))
    {
        return 1.0;
    }
    return 0.0;
}

FEIGN_END()
