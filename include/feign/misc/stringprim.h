#pragma once

#include <feign/node.h>

class StringPrimitive : public SceneNode
{
public:
    StringPrimitive();
    StringPrimitive(SceneNode* parent);
    virtual ~StringPrimitive();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
