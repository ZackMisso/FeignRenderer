#pragma once

#include <feign/node.h>

class BSDF : public SceneNode
{
public:
    BSDF();
    BSDF(SceneNode* parent);
    virtual ~BSDF();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
