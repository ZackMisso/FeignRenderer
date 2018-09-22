#include <feign/cameras/perspective.h>

Perspective::Perspective() : Camera()
{
    // TODO
}

Perspective::Perspective(Node* parent) : Camera(parent)
{
    // TODO
}

void Perspective::preProcess()
{
    throw new NotImplementedException("preprocess persp");

    preProcessChildren();
}

string Perspective::getName() const
{
    return Camera::getName() + "perspective";
}
