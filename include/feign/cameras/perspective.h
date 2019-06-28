#pragma once

#include <feign/cameras/camera.h>

class Perspective : public Camera
{
public:
    Perspective();
    Perspective(Node* parent);
    Perspective(Node* parent,
                Float aperatureRadius,
                Float focalDistance,
                Float fov,
                Float near,
                Float far,
                uint32_t width,
                uint32_t height);

    virtual void preProcess();

    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const;

    virtual std::string getName() const;

    void print() const;

protected:
    Transform sampleToCamera;
    Float aperatureRadius;
    Float focalDistance;
    Float fov;
    Float near;
    Float far;
};
