#pragma once

#include <feign/node.h>

class Integrator : public SceneNode
{
public:
    Integrator();
    Integrator(SceneNode* parent);
    virtual ~Integrator();

    // TODO
    virtual void tempMethod() = 0;

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;
};
