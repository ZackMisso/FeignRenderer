#include <feign/accel/accel.h>

Accel::Accel()
{
    meshes = vector<Shape*>();
}

Accel::~Accel()
{
    meshes.clear();
}

string Accel::getName() const
{
    return "accel: ";
}

NodeType Accel::getNodeType() const
{
    return NT_Accel;
}
