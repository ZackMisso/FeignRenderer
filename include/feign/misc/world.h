#pragma once

// this is a hacky sort of thing to make a global first node

#include <feign/node.h>

class WorldNode : public Node
{
public:
    WorldNode();
    WorldNode(Node* parent);
    virtual ~WorldNode();

    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;
};
