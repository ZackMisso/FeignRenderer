/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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
