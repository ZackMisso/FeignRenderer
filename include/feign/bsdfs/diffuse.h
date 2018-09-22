#pragma once

#include <feign/bsdfs/bsdf.h>

class Diffuse : public BSDF
{
public:
    Diffuse();
    Diffuse(Node* parent);

    virtual void preProcess();

    virtual string getName() const;

    // TODO
};
