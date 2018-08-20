#pragma once

#include <feign/node.h>

class Integrator : public Node
{
public:
    Integrator();
    Integrator(Node* parent);
    virtual ~Integrator();

    // TODO
    virtual void tempMethod() = 0;

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;
};
