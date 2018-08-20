#include <feign/misc/world.h>

WorldNode::WorldNode() : SceneNode()
{
    // does nothing for now
}

WorldNode::WorldNode(SceneNode* parent) : SceneNode(parent)
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
