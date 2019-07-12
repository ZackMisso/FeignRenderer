#include <feign/accel/accel.h>
#include <feign/misc/embree_util.h>

Accel::Accel()
{
    meshes = std::vector<Shape*>();
}

Accel::~Accel()
{
    std::cout << "deleting accel" << std::endl;
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
