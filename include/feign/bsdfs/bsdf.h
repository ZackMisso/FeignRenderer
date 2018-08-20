#pragma once

#include <feign/node.h>

class BSDF : public Node
{
public:
    BSDF();
    BSDF(Node* parent);
    virtual ~BSDF();

    virtual string getName() const;

    virtual NodeType getNodeType() const;

    // TODO
};
