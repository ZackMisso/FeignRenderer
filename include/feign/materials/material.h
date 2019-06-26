#pragma once

#include <feign/common.h>
#include <feign/bsdfs/bsdf.h>

// TODO: eventually make this more complicated
// idk how to structure this, need to research some more on how other renderers
// do this

class Material
{
public:
    Material();
    ~Material();

    BSDF* getBSDF() const { return bsdf; }
    void setBSDF(BSDF* param) { bsdf = param; }

protected:
    BSDF* bsdf;
};
