#pragma once

#include <feign/node.h>

class Emitter : public Node
{
public:
    Emitter();
    Emitter(Node* parent);
    virtual ~Emitter();

    virtual std::string getName() const;

    virtual NodeType getNodeType() const;

    // TODO
};
