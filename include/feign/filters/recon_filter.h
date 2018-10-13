#pragma once

#include <feign/node.h>

class ReconstructionFilter : public Node
{
public:
    ReconstructionFilter();
    ReconstructionFilter(Vec2f size);

    virtual float evaluate(const Point2f& p) const = 0;

protected:
    Vec2f size;
};
