#pragma once

#include <feign/node.h>

class Transform : public Node
{
public:
    Transform();
    Transform(Node* parent);
    ~Transform();

    virtual string getName() const;

    virtual NodeType getNodeType() const;

    // TODO
};
