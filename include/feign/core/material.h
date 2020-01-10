/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/bsdf.h>
#include <feign/misc/intersection.h>

class Material
{
public:
    Material() { }
    virtual ~Material() { }

    // should this get the bsdf???
    // need to do more research on how more complicated materials work
    virtual BSDF* getBSDF(const Intersection& its) const = 0;
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

    virtual BSDF* getBSDF(const Intersection& its) const;

    BSDFNode* bsdf;
};

// a material with different scattering properties along the edges of a mesh
class WireframeMaterial : public Material
{
public:
    struct Params
    {
        Params(std::string wireframe_bsdf,
               std::string mesh_bsdf,
               float threshold)
            : wireframe_bsdf(wireframe_bsdf),
              mesh_bsdf(mesh_bsdf),
              threshold(threshold) { }

        std::string wireframe_bsdf;
        std::string mesh_bsdf;
        float threshold;
    };

    WireframeMaterial(BSDFNode* wireframe_bsdf,
                      BSDFNode* mesh_bsdf,
                      float threshold);
    ~WireframeMaterial();

    virtual BSDF* getBSDF(const Intersection& its) const;

    BSDFNode* wireframe_bsdf;
    BSDFNode* mesh_bsdf;
    float threshold;
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

    Material* material;
};
/////////////////////////////////////////////////
