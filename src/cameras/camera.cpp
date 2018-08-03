#include <feign/cameras/camera.h>

Camera::Camera() : SceneNode()
{
    // TODO
}

Camera::Camera(SceneNode* parent) : SceneNode(parent)
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
