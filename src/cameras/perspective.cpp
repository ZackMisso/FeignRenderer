#include <feign/cameras/perspective.h>

Perspective::Perspective() : Camera()
{
    // TODO
}

Perspective::Perspective(Node* parent) : Camera(parent)
{
    // TODO
}

string Perspective::getName() const
{
    return Camera::getName() + "perspective";
}
