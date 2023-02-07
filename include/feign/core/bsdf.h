/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/closure.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

/////////////////////////////////////////////////
// BSDF
/////////////////////////////////////////////////
class BSDF
{
public:
    BSDF() {}
    virtual ~BSDF() {}

    virtual void sample(MaterialClosure &closure) const = 0;
    virtual void evaluate(MaterialClosure &closure) const = 0;

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
            : albedo(albedo) {}

        Color3f albedo;
    };

    Diffuse(Color3f albedo);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;

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
            : albedo(albedo) {}

        Color3f albedo;
    };

    Mirror(Color3f albedo);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;

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

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;

    virtual bool isDelta() const { return true; }
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Phong BSDF
/////////////////////////////////////////////////
class Phong : public BSDF
{
public:
    struct Params
    {
        Params(Color3f kd, Float ks, Float exponent)
            : kd(kd), ks(ks), exponent(exponent) {}

        Color3f kd;
        Float ks;
        Float exponent;
    };

    Phong(Color3f kd, Float ks, Float exponent);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;

    // this is a strange middle ground
    virtual bool isDelta() const { return false; }

protected:
    Color3f kd;
    Float ks;
    Float exponent;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Blinn BSDF
/////////////////////////////////////////////////
class Blinn : public BSDF
{
public:
    struct Params
    {
        Params(Color3f kd, Color3f ks, Float exponent)
            : kd(kd), ks(ks), exponent(exponent) {}

        Color3f kd;
        Color3f ks;
        Float exponent;
    };

    Blinn(const Blinn::Params *params);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;

    virtual bool isDelta() const { return false; }

protected:
    Color3f kd;
    Color3f ks;
    Float exponent;
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
            : int_ior(int_ior), ext_ior(ext_ior), albedo(Color3f(1.f)) {}

        Params(Float int_ior, Float ext_ior, Color3f albedo)
            : int_ior(int_ior), ext_ior(ext_ior), albedo(albedo) {}

        Float int_ior;
        Float ext_ior;
        Color3f albedo;
    };

    Dielectric(Float int_ior, Float ext_ior, Color3f albedo);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;

    virtual bool isDelta() const { return true; }

protected:
    Float int_ior;
    Float ext_ior;
    Color3f albedo;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// One-Way Dielectric BSDF
//
// This material is not based off of any real material, it is simply a hacky
// BSDF to create a cool scene idea.
/////////////////////////////////////////////////
class OneWayDielectric : public BSDF
{
public:
    struct Params
    {
        Params(Float int_ior, Float ext_ior)
            : int_ior(int_ior), ext_ior(ext_ior), albedo(Color3f(1.f)) {}

        Params(Float int_ior, Float ext_ior, Color3f albedo)
            : int_ior(int_ior), ext_ior(ext_ior), albedo(albedo) {}

        Float int_ior;
        Float ext_ior;
        Color3f albedo;
    };

    OneWayDielectric(Float int_ior, Float ext_ior, Color3f albedo);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;

    virtual bool isDelta() const { return true; }

protected:
    Float int_ior;
    Float ext_ior;
    Color3f albedo;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// BSDF Node structure
/////////////////////////////////////////////////
struct BSDFNode : public Node
{
public:
    BSDFNode() : bsdf(nullptr) {}
    BSDFNode(std::string name) : Node(name), bsdf(nullptr) {}
    BSDFNode(BSDF *bsdf) : bsdf(bsdf) {}

    ~BSDFNode() { delete bsdf; }

    BSDF *operator()() { return bsdf; }

    BSDF *bsdf;
};
/////////////////////////////////////////////////

FEIGN_END()
