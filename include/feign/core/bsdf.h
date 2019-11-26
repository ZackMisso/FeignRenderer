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
              const Point3f& pos);

    BSDFQuery(const Vector3f& wi,
              const Point3f& uvw,
              const Point3f& pos);

    BSDFQuery(const Vector3f& wi,
              const Vector3f& wo,
              const Point2f& uv,
              const Point3f& pos);

    BSDFQuery(const Vector3f& wi,
              const Vector3f& wo,
              const Point3f& uvw,
              const Point3f& pos);
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// BSDF
/////////////////////////////////////////////////
class BSDF : public Node
{
public:
    BSDF();
    virtual ~BSDF();

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const = 0;
    virtual Color3f eval(const BSDFQuery& rec) const = 0;
    virtual Float pdf(const BSDFQuery& rec) const = 0;

    virtual bool isDelta() const = 0;

    virtual std::string getName() const;
    virtual NodeType getNodeType() const;

    // TODO
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Diffuse BSDF
/////////////////////////////////////////////////
class Diffuse : public BSDF
{
public:
    Diffuse(Color3f albedo);

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const;
    virtual Color3f eval(const BSDFQuery& rec) const;
    virtual Float pdf(const BSDFQuery& rec) const;

    virtual bool isDelta() const { return false; }

    virtual std::string getName() const;

protected:
    Color3f albedo;
};
/////////////////////////////////////////////////
