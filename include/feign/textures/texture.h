#pragma once

#include <feign/node.h>

class Texture : public Node
{
public:
    Texture();
    virtual ~Texture();

    virtual string getName() const;

    // TODO
};
