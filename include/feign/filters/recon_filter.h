#pragma once

#include <feign/node.h>

class ReconstructionFilter : public Node
{
public:
    ReconstructionFilter();
    ReconstructionFilter(Vec2f size);

    virtual Float evaluate(const Point2f& p) const = 0;

    virtual NodeType getNodeType() const;

    virtual std::string getName() const;

    Vec2f getSize() const { return size; }

protected:
    Vec2f size;
};
