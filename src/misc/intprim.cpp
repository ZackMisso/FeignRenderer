#include <feign/misc/intprim.h>

IntPrimitive::IntPrimitive() : SceneNode()
{
    // TODO
}

IntPrimitive::IntPrimitive(SceneNode* parent) : SceneNode(parent)
{
    // TODO
}

IntPrimitive::~IntPrimitive()
{
    // TODO
}

NodeType IntPrimitive::getNodeType() const
{
    return NT_Primitive;
}

bool IntPrimitive::isPrimitive() const
{
    return true;
}
