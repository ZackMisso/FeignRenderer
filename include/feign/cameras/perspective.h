#pragma once

#include <feign/cameras/camera.h>

class Perspective : public Camera
{
public:
    Perspective();
    Perspective(Node* parent);

    virtual void preProcess();

    virtual Color3f sampleRay(Ray3f ray,
                              Point2f filmSamp,
                              Point2f appSamp) const;

    virtual string getName() const;

protected:
    Transform sampleToCamera;
    float aperatureRadius;
    float focalDistance;
    float fov;
    float near;
    float far;
};
