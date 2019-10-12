/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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
