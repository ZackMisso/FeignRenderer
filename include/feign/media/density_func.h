/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/bbox.h>

#if OPENVDB

#include <openvdb/openvdb.h>
#include <openvdb/tools/ChangeBackground.h>

#endif

FEIGN_BEGIN()

// TODO: need to redesign entire renderer to run off macros instead of having
//       both spectral and normal support
class DensityFunction
{
public:
    virtual ~DensityFunction() { }
    virtual Float D(const Point3f& p) const = 0;
    virtual Color3f SpectralD(const Point3f& p) const = 0;
    virtual Float maxDensity() const = 0;

    virtual void preProcess() { }

    Color3f sigma_t;
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
        // assert(false);
        return std::max(density(0) * sigma_t(0),
               std::max(density(1) * sigma_t(1),
                        density(2) * sigma_t(2)));
    }

    virtual Color3f SpectralD(const Point3f& p) const
    {
        return density * sigma_t;
    }

    virtual Float maxDensity() const
    {
        return std::max(density(0) * sigma_t(0),
               std::max(density(1) * sigma_t(1),
                        density(2) * sigma_t(2)));
    }

    Color3f density;
};

#if OPENVDB

class OpenVDBDensity : public DensityFunction
{
public:
    struct Params
    {
        Params(std::string filename)
            : filename(filename) { }

        std::string filename;
    };

    OpenVDBDensity(std::string openvdb_file);
    ~OpenVDBDensity();

    virtual Float D(const Point3f& p) const;
    virtual Color3f SpectralD(const Point3f& p) const;
    virtual Float maxDensity() const;

    virtual void preProcess();

    // TODO: try to also support spectral grids
    openvdb::FloatGrid::Ptr grid;
    // openvdb::Coord min;
    // openvdb::Coord max;

    BBox3f bbox;
    Float max_density;
};

#endif

// TODO: still need to design this class. What are the correct noise parameters
//       to use here?
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

class PointAverageDensity : public DensityFunction
{
    PointAverageDensity();

    virtual Float D(const Point3f& p) const;
    virtual Color3f SpectralD(const Point3f& p) const;
    virtual Float maxDensity() const;

    // TODO
};

class MandlebrotDensity : public DensityFunction
{
public:
    MandlebrotDensity()
    {
        // TODO
    }

    virtual Float D(const Point3f& p) const
    {
        throw new NotImplementedException("mandlebrot density");
        return 0.0;
    }

    virtual Color3f SpectralD(const Point3f& p) const
    {
        throw new NotImplementedException("mandlebrot density");
        return Color3f(0.f);
    }

    virtual Float maxDensity() const
    {
        throw new NotImplementedException("mandlebrot density");
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

FEIGN_END()
