#pragma once

#include <feign/node.h>

class Emitter : public Node
{
public:
    Emitter();
    Emitter(Node* parent);
    virtual ~Emitter();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
