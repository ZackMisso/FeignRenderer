#pragma once

#include <feign/cameras/camera.h>

class Perspective : public Camera
{
public:
    Perspective();
    Perspective(Node* parent);

    virtual string getName() const;

    // TODO
};
