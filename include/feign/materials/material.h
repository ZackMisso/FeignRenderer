#pragma once

#include <feign/common.h>
#include <feign/bsdf/bsdf.h>

// TODO: eventually make this more complicated
// idk how to structure this

class Material
{
public:
    Material();
    ~Material();

    const BSDF* getBSDF() const { return bsdf; }
    void setBSDF(BSDF* param) { bsdf = param; }

protected:
    BSDF* bsdf;
};
