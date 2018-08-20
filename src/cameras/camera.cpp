#include <feign/cameras/camera.h>

Camera::Camera() : Node()
{
    // TODO
}

Camera::Camera(Node* parent) : Node(parent)
{
    // TODO
}

Camera::~Camera()
{
    // TODO
}

NodeType Camera::getNodeType() const
{
    return NT_Camera;
}

bool Camera::isPrimitive() const
{
    return false;
}
