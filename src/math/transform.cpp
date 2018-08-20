#include <feign/math/transform.h>

Transform::Transform() : Node()
{
    // TODO
}

Transform::Transform(Node* parent) : Node(parent)
{
    // TODO
}

Transform::~Transform()
{
    // TODO
}

string Transform::getName() const
{
    return "transform";
}

NodeType Transform::getNodeType() const
{
    return NT_Transform;
}
