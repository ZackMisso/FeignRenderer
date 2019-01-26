#include <feign/accel/accel.h>

Accel::Accel()
{
    meshes = std::vector<Shape*>();
}

Accel::~Accel()
{
    meshes.clear();
}

std::string Accel::getName() const
{
    return "accel: ";
}

NodeType Accel::getNodeType() const
{
    return NT_Accel;
}

void Accel::setMeshes(const std::vector<Shape*>& param)
{
    meshes = param;
}
