#include <feign/misc/floatprim.h>

FloatPrimitive::FloatPrimitive() : SceneNode()
{
    // TODO
}

FloatPrimitive::FloatPrimitive(SceneNode* parent) : SceneNode(parent)
{
    // TODO
}

FloatPrimitive::~FloatPrimitive()
{
    // TODO
}

NodeType FloatPrimitive::getNodeType() const
{
    return NT_Primitive;
}

bool FloatPrimitive::isPrimitive() const
{
    return true;
}
