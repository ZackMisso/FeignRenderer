#pragma once

#include <feign/node.h>

class Texture : public Node
{
public:
    Texture();
    virtual ~Texture();

    virtual void preProcess();

    virtual std::string getName() const;

    // TODO
};
