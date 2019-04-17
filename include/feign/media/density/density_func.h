#pragma once

#include <feign/common.h>

class Density_Function
{
    virtual Float D(const Point3f& p) const = 0;
    virtual Color3f SpectralD(const Point3f& p) const = 0;
};
