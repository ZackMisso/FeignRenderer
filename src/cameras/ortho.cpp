#include <feign/cameras/ortho.h>

Orthographic::Orthographic() : Camera()
{
    // TODO
}

Orthographic::Orthographic(Node* parent) : Camera(parent)
{
    // TODO
}

void Orthographic::preProcess(bool use_prims)
{
    throw new NotImplementedException("preprocess ortho");

    preProcessChildren(use_prims);
}

Color3f Orthographic::sampleRay(Ray3f& ray,
                                const Point2f& filmSamp,
                                const Point2f& appSamp) const
{
    throw new NotImplementedException("ortho sampleRay");

    return Color3f(1.0);
}

std::string Orthographic::getName() const
{
    return Camera::getName() + " orthographic";
}
