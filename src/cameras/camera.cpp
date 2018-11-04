#include <feign/cameras/camera.h>

Camera::Camera() : Node() { }

Camera::Camera(Node* parent) : Node(parent) { }

Camera::~Camera() { }

Vec2i Camera::getFilmSize() const
{
    return filmSize;
}

string Camera::getName() const
{
    return "camera: ";
}

NodeType Camera::getNodeType() const
{
    return NT_Camera;
}
