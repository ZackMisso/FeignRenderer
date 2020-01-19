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

class Material
{
public:
    Material() { }
    virtual ~Material() { }

    virtual void sample(MaterialClosure& closure) const = 0;
    virtual void evaluate(MaterialClosure& closure) const = 0;

    virtual bool isDelta() const { return false; }
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
