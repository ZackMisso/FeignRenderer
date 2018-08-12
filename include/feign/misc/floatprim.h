#pragma once

#include <feign/node.h>

class FloatPrimitive : public SceneNode
{
public:
    FloatPrimitive();
    FloatPrimitive(SceneNode* parent);
    virtual ~FloatPrimitive();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
