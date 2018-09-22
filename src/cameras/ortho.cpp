#include <feign/cameras/ortho.h>

Orthographic::Orthographic() : Camera()
{
    // TODO
}

Orthographic::Orthographic(Node* parent) : Camera(parent)
{
    // TODO
}

void Orthographic::preProcess()
{
    throw new NotImplementedException("preprocess ortho");

    preProcessChildren();
}

string Orthographic::getName() const
{
    return Camera::getName() + " orthographic";
}
