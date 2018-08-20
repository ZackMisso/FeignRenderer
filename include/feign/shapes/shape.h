#pragma once

#include <feign/node.h>

class Shape : public Node
{
public:
    Shape();
    Shape(Node* parent);
    virtual ~Shape();

    virtual string getName() const;

    virtual NodeType getNodeType() const;
};
