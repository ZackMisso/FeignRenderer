#pragma once

#include <feign/node.h>
#include <imedit/image.h>

class Texture : public Node
{
public:
    Texture();
    virtual ~Texture();

    virtual void preProcess();

    virtual string getName() const;

    // TODO
};
