#include <feign/shapes/shape.h>

Shape::Shape() : Node()
{
    // cout << "SHAPE Constructor" << endl;
    // TODO
    // cout << "END SHAPE" << endl;
}

Shape::Shape(Node* parent) : Node(parent)
{
    // TODO
}

Shape::~Shape()
{
    // TODO
}

string Shape::getName() const
{
    return "mesh: ";
}

NodeType Shape::getNodeType() const
{
    return NT_Mesh;
}
