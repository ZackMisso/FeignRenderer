#pragma once

#include <feign/node.h>

class Sampler : public SceneNode
{
public:
    Sampler();
    Sampler(SceneNode* parent);
    virtual ~Sampler();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
