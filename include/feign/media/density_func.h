/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

class DensityFunction
{
    virtual Float D(const Point3f& p) const = 0;
    virtual Color3f SpectralD(const Point3f& p) const = 0;
};

class HomogeneousDensity : public DensityFunction
{
public:
    HomogeneousDensity(Color3f density) : density(density) { }
    HomogeneousDensity(Float density_val) : density(Color3f(density_val)) { }

    virtual Float D(const Point3f& p) const
    {
        return density(0);
    }

    virtual Color3f SpectralD(const Point3f& p) const
    {
        return density;
    }

    Color3f density;
};

class OpenVDBDensity : public DensityFunction
{
public:
    OpenVDBDensity(std::string openvdb_file)
    {
        throw new NotImplementedException("openvdb density");
        // TODO
    }

    virtual Float D(const Point3f& p) const
    {
        throw new NotImplementedException("openvdb density");
        return 0.0;
    }

    virtual Color3f SpectralD(const Point3f& p) const
    {
        throw new NotImplementedException("openvdb density");
        return Color3f(0.f);
    }
};

class NoiseDensity : public DensityFunction
{
public:
    NoiseDensity()
    {
        // TODO
    }

    virtual Float D(const Point3f& p) const
    {
        throw new NotImplementedException("noise density");
        return 0.0;
    }

    virtual Color3f SpectralD(const Point3f& p) const
    {
        throw new NotImplementedException("noise density");
        return Color3f(0.f);
    }
};
