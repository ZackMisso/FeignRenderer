#pragma once

#include <feign/node.h>

class Sampler : public Node
{
public:
    Sampler();
    Sampler(Node* parent);
    virtual ~Sampler();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
