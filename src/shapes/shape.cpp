#include <feign/shapes/shape.h>

Shape::Shape() : Node()
{
    material = new Material();
}

Shape::Shape(Node* parent) : Node(parent)
{
    material = new Material();
}

Shape::~Shape()
{
    delete material;
}

std::string Shape::getName() const
{
    return "mesh: ";
}

NodeType Shape::getNodeType() const
{
    return NT_Mesh;
}
