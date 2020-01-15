/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

class Density_Function
{
    virtual Float D(const Point3f& p) const = 0;
    virtual Color3f SpectralD(const Point3f& p) const = 0;
};
