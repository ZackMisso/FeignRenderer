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

string Camera::getName() const
{
    return "camera: ";
}


NodeType Camera::getNodeType() const
{
    return NT_Camera;
}
