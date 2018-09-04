#include <feign/cameras/ortho.h>

Orthographic::Orthographic() : Camera()
{
    // TODO
}

Orthographic::Orthographic(Node* parent) : Camera(parent)
{
    // TODO
}

string Orthographic::getName() const
{
    return Camera::getName() + " orthographic";
}
