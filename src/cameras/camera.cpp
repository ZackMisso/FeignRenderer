#include <feign/cameras/camera.h>

Camera::Camera() : Node() { }

Camera::Camera(Node* parent) : Node(parent) { }

Camera::~Camera() { std::cout << "deleting camera" << std::endl; }

Vec2i Camera::getFilmSize() const
{
    return filmSize;
}

std::string Camera::getName() const
{
    return "camera: ";
}

NodeType Camera::getNodeType() const
{
    return NT_Camera;
}
