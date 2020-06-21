/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

// TODO: need to redesign entire renderer to run off macros instead of having
//       both spectral and normal support
class DensityFunction
{
public:
    virtual ~DensityFunction() { }
    virtual Float D(const Point3f& p) const = 0;
    virtual Color3f SpectralD(const Point3f& p) const = 0;
    virtual Float maxDensity() const = ;
};

class ConstantDensity : public DensityFunction
{
public:
    struct Params
    {
        Params(Float density)
            : density(density) { }
        Params(Color3f density)
            : density(density) { }

        Color3f density;
    };

    ConstantDensity(Color3f density) : density(density) { }
    ConstantDensity(Float density_val) : density(Color3f(density_val)) { }

    virtual Float D(const Point3f& p) const
    {
        return density(0);
    }

    virtual Color3f SpectralD(const Point3f& p) const
    {
        return density;
    }

    virtual Float maxDensity() const
    {
        return std::max(density(0), std::max(density(1), density(2)));
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

    virtual Float maxDensity() const
    {
        throw new NotImplementedException("openvdb density");
        return 0.f;
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

    virtual Float maxDensity() const
    {
        throw new NotImplementedException("noise density");
        return 0.f;
    }
};

// this is just a simple density which is going to be used to test
class SphereDensity : public DensityFunction
{
public:
    struct Params
    {
        Params(Float density, Float radius)
            : density(density), radius(radius) { }
        Params(Color3f density, Float radius)
            : density(density), radius(radius) { }

        Color3f density;
        Float radius;
    };

    SphereDensity(Params* params)
        : density(params->density), radius(params->radius) { }

    virtual Float D(const Point3f& p) const
    {
        if (p.sqrNorm() < radius*radius) return density(0);
        return 0.f;
    }

    virtual Color3f SpectralD(const Point3f& p) const
    {
        if (p.sqrNorm() < radius*radius) return density;
        return Color3f(0.f);
    }

    virtual Float maxDensity() const
    {
        return std::max(density(0), std::max(density(1), density(2)));
    }

    Color3f density;
    Float radius;
};

/////////////////////////////////////////////////
// DensityFunction Node structure
/////////////////////////////////////////////////
struct DensityFunctionNode : public Node
{
public:
    DensityFunctionNode() : density(nullptr) { }
    DensityFunctionNode(std::string name) : Node(name), density(nullptr) { }
    DensityFunctionNode(DensityFunction* density) : density(density) { }

    ~DensityFunctionNode() { delete density; }

    DensityFunction* density;
};
/////////////////////////////////////////////////
