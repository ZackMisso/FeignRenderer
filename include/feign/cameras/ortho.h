#pragma once

#include <feign/cameras/camera.h>

class Orthographic : public Camera
{
public:
    Orthographic();
    Orthographic(Node* parent);

    virtual string getName() const;

    // TODO
};
