#pragma once

#include <feign/node.h>

// TODO: make an abstraction for 2D, 3D, and Volumetrix textures
class Texture : public Node
{
public:
    Texture();
    virtual ~Texture();

    virtual void preProcess();

    virtual std::string getName() const;

    // TODO
};
