/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

/////////////////////////////////////////////////
// BSDF Query Object
/////////////////////////////////////////////////
struct BSDFQuery
{
    Vector3f wi;
    Vector3f wo;

    Point3f uvw;
    Point3f pos;

    Float eta;

    bool isDiffuse;

    BSDFQuery(const Vector3f& wi,
              const Point2f& uv,
              const Point3f& pos)
        : wi(wi), uvw(Point3f(uv, 0.f)), pos(pos) { }

    BSDFQuery(const Vector3f& wi,
              const Point3f& uvw,
              const Point3f& pos)
        : wi(wi), uvw(uvw), pos(pos) { }

    BSDFQuery(const Vector3f& wi,
              const Vector3f& wo,
              const Point2f& uv,
              const Point3f& pos)
        : wi(wi), wo(wo), uvw(Point3f(uv, 0.f)), pos(pos) { }

    BSDFQuery(const Vector3f& wi,
              const Vector3f& wo,
              const Point3f& uvw,
              const Point3f& pos)
        : wi(wi), wo(wo), uvw(uvw), pos(pos) { }
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// BSDF
/////////////////////////////////////////////////
class BSDF
{
public:
    BSDF() { }
    virtual ~BSDF() { }

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const = 0;
    virtual Color3f eval(const BSDFQuery& rec) const = 0;
    virtual Float pdf(const BSDFQuery& rec) const = 0;

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

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    virtual Color3f eval(const BSDFQuery& rec) const;
    virtual Float pdf(const BSDFQuery& rec) const;

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

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    virtual Color3f eval(const BSDFQuery& rec) const;
    virtual Float pdf(const BSDFQuery& rec) const;

    virtual bool isDelta() const { return true; }

protected:
    Color3f albedo;
};
/////////////////////////////////////////////////

// TODO: this should not be a bsdf and should instead be a material
// /////////////////////////////////////////////////
// // Wireframe BSDF
// // this BSDF was used for the jan_3_2020 shader. It is a diffuse
// // shader whose albedo is line_albedo along lines
// /////////////////////////////////////////////////
// class Wireframe : public BSDF
// {
// public:
//     Wireframe(Color3f albedo);
//
//     virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
//     virtual Color3f eval(const BSDFQuery& rec) const;
//     virtual Float pdf(const BSDFQuery& rec) const;
//
//     virtual bool isDelta() const { return true; }
//
// protected:
//     Color3f albedo;
// };
// /////////////////////////////////////////////////

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

    BSDF* bsdf;
};
/////////////////////////////////////////////////
