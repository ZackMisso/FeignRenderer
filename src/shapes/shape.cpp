#include <feign/shapes/shape.h>

Shape::Shape() : Node()
{
    // TODO
}

Shape::Shape(Node* parent) : Node(parent)
{
    // TODO
}

Shape::~Shape()
{
    // TODO
}

NodeType Shape::getNodeType() const
{
    return NT_Mesh;
}

bool Shape::isPrimitive() const
{
    return false;
}
