#pragma once

#include <feign/node.h>

class ReconstructionFilter : public Node
{
public:
    ReconstructionFilter();
    ReconstructionFilter(Vec2f size);

    virtual float evaluate(const Point2f& p) const = 0;

    virtual NodeType getNodeType() const;

    virtual std::string getName() const;

protected:
    Vec2f size;
};
