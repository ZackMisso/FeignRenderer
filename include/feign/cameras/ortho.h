#pragma once

#include <feign/cameras/camera.h>

class Orthographic : public Camera
{
public:
    Orthographic();
    Orthographic(Node* parent);

    virtual void preProcess();

    virtual Color3f sampleRay(Ray3f& ray,
                              const Point2f& filmSamp,
                              const Point2f& appSamp) const;

    virtual string getName() const;
};
