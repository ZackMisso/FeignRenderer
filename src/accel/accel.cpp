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

void Accel::setMeshes(const vector<Shape*>& param)
{
    meshes = param;
}
