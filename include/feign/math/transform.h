#pragma once

#include <feign/node.h>

class Transform : public SceneNode
{
public:
    Transform();
    ~Transform();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
