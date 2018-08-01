#pragma once

#include <feign/node.h>

class BRDF : public SceneNode
{
public:
    BRDF();
    BRDF(SceneNode* parent);
    virtual ~BRDF();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
