#include <feign/filters/box_filter.h>

BoxFilter::BoxFilter() : ReconstructionFilter()
{
    // does nothing
}

BoxFilter::BoxFilter(Vec2f size) : ReconstructionFilter(size)
{
    // does nothing
}

void BoxFilter::preProcess()
{
    throw new NotImplementedException("box_filter preProcess");
}

float BoxFilter::evaluate(const Point2f& p) const
{
    throw new NotImplementedException("box_filter evaluate");
    return 1.f;
}

string BoxFilter::getName() const
{
    return ReconstructionFilter::getName() + "box";
}