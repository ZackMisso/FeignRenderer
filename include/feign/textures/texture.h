/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

 // TODO: is this really worth an abstraction if I am going to implement a
 //       shader system?

#pragma once

#include <feign/core/node.h>

// TODO: make an abstraction for 2D, 3D, and Volumetrix textures
class Texture : public Node
{
public:
    Texture();
    virtual ~Texture();

    virtual void evaluate(const Point3f& point) = 0;

    virtual void preProcess();

    virtual std::string getName() const;

    // TODO
};
