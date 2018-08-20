#pragma once

#include <feign/node.h>

class BSDF : public Node
{
public:
    BSDF();
    BSDF(Node* parent);
    virtual ~BSDF();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // TODO
};
