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
#include <feign/math/complex.h>

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
    virtual Color3f D(const Point3f& p) const = 0;
    // virtual Color3f SpectralD(const Point3f& p) const = 0;
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

    virtual Color3f D(const Point3f& p) const
    {
        return density * sigma_t;
    }

    // virtual Color3f SpectralD(const Point3f& p) const
    // {
    //     return density * sigma_t;
    // }

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

    virtual Color3f D(const Point3f& p) const;
    // virtual Color3f SpectralD(const Point3f& p) const;
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

    virtual Color3f D(const Point3f& p) const
    {
        throw new NotImplementedException("noise density");
        return 0.0;
    }

    // virtual Color3f SpectralD(const Point3f& p) const
    // {
    //     throw new NotImplementedException("noise density");
    //     return Color3f(0.f);
    // }

    virtual Float maxDensity() const
    {
        throw new NotImplementedException("noise density");
        return 0.f;
    }
};

class PointAverageDensity : public DensityFunction
{
    PointAverageDensity();

    virtual Color3f D(const Point3f& p) const;
    // virtual Color3f SpectralD(const Point3f& p) const;
    virtual Float maxDensity() const;

    // TODO
};

class MandlebrotDensity : public DensityFunction
{
public:
    struct Params
    {
        Params(Color3f max_density)
            : max_density(max_density) { }

        Color3f max_density;
    };

    MandlebrotDensity(Color3f max_density)
        : max_density(max_density),
          center(Point3f(0.f)),
          bounds(1000.0),
          n(1000) { }

    // z(n+1) = z(n) + c
    // c will be defined to be the location of the point relative to the center
    virtual Color3f D(const Point3f& p) const
    {
        // initial implementation: if z(n+1) is bounded for a specific n, return
        // 1.f otherwise return 0.f;

        Complex3f c = Complex3f(p(0) * 0.0625, p(1) * 0.0625, p(2) * 0.0625);
        // Complex3f c = Complex3f(-2.1);

        // double x_c = (p(0) - center(0)) * 0.5;
        // double y_c = (p(1) - center(1)) * 0.5;
        // double z_c = (p(2) - center(2)) * 0.5;

        Complex3f val = Complex3f(0.f, 0.f, 0.f);

        // double x_val = 0.f;
        // double y_val = 0.f;
        // double z_val = 0.f;

        int m = -1;
        int m2 = -1;
        int m3 = -1;

        double bounds_1 = 0.1;
        double bounds_2 = 0.5;
        double bounds_3 = 2.0;

        while (val.norm() <= bounds_1 &&
               m < n)
        {
            val = val * val + c;
            m++;
        }

        val = Complex3f(0.f, 0.f, 0.f);

        while (val.norm() <= bounds_2 &&
               m2 < n)
        {
            val = val * val + c;
            m2++;
        }

        val = Complex3f(0.f, 0.f, 0.f);

        while (val.norm() <= bounds_3 &&
               m3 < n)
        {
            val = val * val + c;
            m3++;
        }

        // for (int i = 0; i < n; ++i)
        // {
        //     x_val = x_val * x_val + x_c;
        //     y_val = y_val * y_val + y_c;
        //     z_val = z_val * z_val + z_c;
        //
        //     if (std::abs(x_val) > bounds) return Color3f(0.f);
        //     if (std::abs(y_val) > bounds) return Color3f(0.f);
        //     if (std::abs(z_val) > bounds) return Color3f(0.f);
        // }

        return Color3f(Float(m) / Float(n),
                       Float(m2) / Float(n),
                       Float(m3) / Float(n));
    }

    // virtual Color3f SpectralD(const Point3f& p) const
    // {
    //     throw new NotImplementedException("mandlebrot density");
    //     return Color3f(0.f);
    // }

    virtual Float maxDensity() const
    {
        throw new NotImplementedException("mandlebrot density");
        return 0.f;
    }

    Color3f max_density;
    Point3f center;
    double bounds;
    int n;
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

    virtual Color3f D(const Point3f& p) const
    {
        if (p.sqrNorm() < radius*radius) return density(0);
        return 0.f;
    }

    // virtual Color3f SpectralD(const Point3f& p) const
    // {
    //     if (p.sqrNorm() < radius*radius) return density;
    //     return Color3f(0.f);
    // }

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
