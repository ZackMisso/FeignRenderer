#include <feign/misc/world.h>

WorldNode::WorldNode() : Node()
{
    // does nothing for now
}

WorldNode::WorldNode(Node* parent) : Node(parent)
{
    // does nothing for now
}

WorldNode::~WorldNode()
{
    // does nothing for now
}

NodeType WorldNode::getNodeType() const
{
    // uhhh i guess
    return NT_Primitive;
}

bool WorldNode::isPrimitive() const
{
    return false;
}
