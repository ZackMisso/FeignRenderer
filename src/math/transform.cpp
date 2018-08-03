#include <feign/math/transform.h>

Transform::Transform()
{
    // TODO
}

Transform::~Transform()
{
    // TODO
}

NodeType Transform::getNodeType() const
{
    return NT_Transform;
}

bool Transform::isPrimitive() const
{
    return false;
}
