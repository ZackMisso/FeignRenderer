#pragma once

#include <feign/node.h>

class Emitter : public SceneNode
{
public:
    Emitter();
    Emitter(SceneNode* parent);
    virtual ~Emitter();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
