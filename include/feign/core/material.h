/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/bsdf.h>
#include <feign/core/closure.h>
#include <feign/misc/intersection.h>

// TODO: should materials and shaders be the same abstraction instead of separate?
//       this really seems annoying that materials do not seem to do much and to me
//       appears like they are just lower level shaders.
class Material
{
public:
    Material() : accepts_shadows(true) { }
    virtual ~Material() { }

    virtual void sample(MaterialClosure& closure) const = 0;
    virtual void evaluate(MaterialClosure& closure) const = 0;

    virtual bool isDelta() const { return false; }

    bool accepts_shadows;
};

// a material with only one bsdf
class SimpleMaterial : public Material
{
public:
    struct Params
    {
        Params(std::string bsdf_name)
            : bsdf_name(bsdf_name) { }

        std::string bsdf_name;
    };

    SimpleMaterial(BSDFNode* bsdf);
    ~SimpleMaterial();

    virtual void sample(MaterialClosure& closure) const;
    virtual void evaluate(MaterialClosure& closure) const;
    virtual bool isDelta() const;

    BSDFNode* bsdf;
};

/////////////////////////////////////////////////
// Material Node structure
/////////////////////////////////////////////////
struct MaterialNode : public Node
{
public:
    MaterialNode() : material(nullptr) { }
    MaterialNode(std::string name) : Node(name), material(nullptr) { }
    MaterialNode(Material* material) : material(material) { }

    ~MaterialNode() { delete material; }

    Material* operator()() { return material; }

    Material* material;
};
/////////////////////////////////////////////////
