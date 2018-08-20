#pragma once

// this is a hacky sort of thing to make a global first node

#include <feign/node.h>

class WorldNode : public Node
{
public:
    WorldNode();
    WorldNode(Node* parent);
    virtual ~WorldNode();

    virtual string getName() const;

    virtual NodeType getNodeType() const;
};
