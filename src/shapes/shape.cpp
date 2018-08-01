#include <feign/shapes/shape.h>

Shape::Shape() : SceneNode()
{
    // TODO
}

Shape::Shape(SceneNode* parent) : SceneNode(parent)
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
