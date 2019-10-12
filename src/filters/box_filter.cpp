/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/filters/box_filter.h>

BoxFilter::BoxFilter() : ReconstructionFilter()
{
    size = Vec2f(0.5, 0.5);
}

BoxFilter::BoxFilter(Vec2f size) : ReconstructionFilter(size) { }

void BoxFilter::preProcess()
{
    // does nothing
}

Float BoxFilter::evaluate(const Point2f& p) const
{
    if (std::abs(p(0)) < size(0) && std::abs(p(1)) < size(1))
    {
        return 1.0;
    }
    return 0.0;
}

std::string BoxFilter::getName() const
{
    return ReconstructionFilter::getName() + "box";
}
