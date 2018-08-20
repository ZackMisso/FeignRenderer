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

    virtual string getName() const;

    virtual NodeType getNodeType() const;
};
