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

    primitives->findTransform("toWorld", cameraToWorld);
    primitives->findInt("width", filmSize[0]);
    primitives->findInt("height", filmSize[1]);
    primitives->findFloat("aperatureRadius", aperatureRadius);
    primitives->findFloat("focalDistance", focalDistance);
    primitives->findFloat("fov", fov);
    primitives->findFloat("nearClip", near);
    primitives->findFloat("farClip", far);

    filter = nullptr;
}

Color3f Perspective::sampleRay(Ray3f ray,
                               Point2f filmSamp,
                               Point2f appSamp) const
{
    throw new NotImplementedException("ortho sampleRay");

    return Color3f(1.0);
}

string Perspective::getName() const
{
    return Camera::getName() + "perspective";
}
