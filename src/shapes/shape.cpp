#include <feign/shapes/shape.h>
#include <feign/shapes/shape.h>

Shape::Shape() : Node() { }

Shape::Shape(Node* parent) : Node(parent) { }

Shape::~Shape() { }

string Shape::getName() const
{
    return "mesh: ";
}

NodeType Shape::getNodeType() const
{
    return NT_Mesh;
}
