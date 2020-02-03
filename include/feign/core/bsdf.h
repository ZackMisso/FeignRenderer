/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/closure.h>

// /////////////////////////////////////////////////
// // BSDF Query Object
// /////////////////////////////////////////////////
// struct BSDFQuery
// {
//     Vector3f wi;
//     Vector3f wo;
//
//     Point3f uvw;
//     Point3f pos;
//
//     Float eta;
//     Float pdf;
//
//     bool isDelta;
//
//     BSDFQuery(const Vector3f& wi,
//               const Point2f& uv,
//               const Point3f& pos)
//         : wi(wi), uvw(Point3f(uv, 0.f)), pos(pos) { }
//
//     BSDFQuery(const Vector3f& wi,
//               const Point3f& uvw,
//               const Point3f& pos)
//         : wi(wi), uvw(uvw), pos(pos) { }
//
//     BSDFQuery(const Vector3f& wi,
//               const Vector3f& wo,
//               const Point2f& uv,
//               const Point3f& pos)
//         : wi(wi), wo(wo), uvw(Point3f(uv, 0.f)), pos(pos) { }
//
//     BSDFQuery(const Vector3f& wi,
//               const Vector3f& wo,
//               const Point3f& uvw,
//               const Point3f& pos)
//         : wi(wi), wo(wo), uvw(uvw), pos(pos) { }
// };
// /////////////////////////////////////////////////

/////////////////////////////////////////////////
// BSDF
/////////////////////////////////////////////////
class BSDF
{
public:
    BSDF() { }
    virtual ~BSDF() { }

    // virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const = 0;
    // virtual Color3f eval(const BSDFQuery& rec) const = 0;

    virtual void sample(MaterialClosure& closure) const = 0;
    virtual void evaluate(MaterialClosure& closure) const = 0;

    // TODO: remove this method
    virtual bool isDelta() const = 0;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Diffuse BSDF
/////////////////////////////////////////////////
class Diffuse : public BSDF
{
public:
    struct Params
    {
        Params(Color3f albedo)
            : albedo(albedo) { }

        Color3f albedo;
    };

    Diffuse(Color3f albedo);

    // virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    // virtual Color3f eval(const BSDFQuery& rec) const;

    virtual void sample(MaterialClosure& closure) const;
    virtual void evaluate(MaterialClosure& closure) const;

    virtual bool isDelta() const { return false; }

protected:
    Color3f albedo;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Mirror BSDF
/////////////////////////////////////////////////
class Mirror : public BSDF
{
public:
    struct Params
    {
        Params(Color3f albedo)
            : albedo(albedo) { }

        Color3f albedo;
    };

    Mirror(Color3f albedo);

    // virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    // virtual Color3f eval(const BSDFQuery& rec) const;

    virtual void sample(MaterialClosure& closure) const;
    virtual void evaluate(MaterialClosure& closure) const;

    virtual bool isDelta() const { return true; }

protected:
    Color3f albedo;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Null BSDF
/////////////////////////////////////////////////
class NullBSDF : public BSDF
{
public:
    NullBSDF();

    // virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    // virtual Color3f eval(const BSDFQuery& rec) const;

    virtual void sample(MaterialClosure& closure) const;
    virtual void evaluate(MaterialClosure& closure) const;

    virtual bool isDelta() const { return true; }
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Dielectric BSDF
/////////////////////////////////////////////////
class Dielectric : public BSDF
{
public:
    struct Params
    {
        Params(Float int_ior, Float ext_ior)
            : int_ior(int_ior), ext_ior(ext_ior) { }

        Float int_ior;
        Float ext_ior;
    };

    Dielectric(Float int_ior, Float ext_ior);

    virtual void sample(MaterialClosure& closure) const;
    virtual void evaluate(MaterialClosure& closure) const;

    virtual bool isDelta() const { return true; }

protected:
    Float int_ior;
    Float ext_ior;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// BSDF Node structure
/////////////////////////////////////////////////
struct BSDFNode : public Node
{
public:
    BSDFNode() : bsdf(nullptr) { }
    BSDFNode(std::string name) : Node(name), bsdf(nullptr) { }
    BSDFNode(BSDF* bsdf) : bsdf(bsdf) { }

    ~BSDFNode() { delete bsdf; }

    BSDF* operator()() { return bsdf; }

    BSDF* bsdf;
};
/////////////////////////////////////////////////
