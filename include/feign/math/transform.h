#pragma once

#include <feign/node.h>

class Transform : public Node
{
public:
    Transform();
    Transform(Node* parent);
    ~Transform();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
