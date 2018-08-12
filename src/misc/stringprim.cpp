#include <feign/misc/stringprim.h>

StringPrimitive::StringPrimitive() : SceneNode()
{
    // TODO
}

StringPrimitive::StringPrimitive(SceneNode* parent) : SceneNode()
{
    // TODO
}

NodeType StringPrimitive::getNodeType() const
{
    return NT_Primitive;
}

bool StringPrimitive::isPrimitive() const
{
    return true;
}
