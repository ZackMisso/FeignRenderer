#pragma once

#include <feign/cameras/camera.h>

class Perspective : public Camera
{
public:
    Perspective();
    Perspective(Node* parent);

    virtual void preProcess();

    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const;

    virtual std::string getName() const;

protected:
    Transform sampleToCamera;
    float aperatureRadius;
    float focalDistance;
    float fov;
    float near;
    float far;
};
