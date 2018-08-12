#pragma once

#include <feign/node.h>

class IntPrimitive : public SceneNode
{
public:
    IntPrimitive();
    IntPrimitive(SceneNode* parent);
    virtual ~IntPrimitive();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
