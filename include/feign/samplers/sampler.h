#pragma once

#include <feign/node.h>

class Sampler : public Node
{
public:
    Sampler();
    Sampler(Node* parent);
    virtual ~Sampler();

    virtual string getName() const;

    virtual NodeType getNodeType() const;

    // TODO
};
