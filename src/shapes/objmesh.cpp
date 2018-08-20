#include <feign/shapes/objmesh.h>

ObjMesh::ObjMesh() : Shape()
{
    // TODO
}

ObjMesh::ObjMesh(Node* parent) : Shape(parent)
{
    // TODO
}

string ObjMesh::getName() const
{
    return Shape::getName() + "obj";
}
