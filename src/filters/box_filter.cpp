#include <feign/filters/box_filter.h>

BoxFilter::BoxFilter() : ReconstructionFilter()
{
    // does nothing
}

BoxFilter::BoxFilter(Vec2f size) : ReconstructionFilter(size)
{
    // does nothing
}

float BoxFilter::evaluate(const Point2f p) const
{
    return 1.f;
}
