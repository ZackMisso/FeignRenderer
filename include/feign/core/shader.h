/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/material.h>
#include <feign/misc/intersection.h>

// material shaders manipulate and control the actual materials which are on objects.
// These shaders are also the entry point for the information which integrators
// get while evaluating materials.
struct MaterialShader
{
public:
    virtual ~MaterialShader() { }

    virtual MaterialClosure evaluate(const Intersection& its) const = 0;
};

// a simple material shader which just evaluates one material
struct SimpleMaterialShader
{
    struct Params
    {
        Params(std::string material)
            : material(material) { }

        std::string material;
    };

    SimpleMaterialShader(MaterialNode* material);

    virtual MaterialClosure evaluate(const Intersection& its) const;

    MaterialNode* material;
};

// geometry shaders manipulate the vertices, normals, and triangles directly.
// These shaders do not manipulate the lighting but instead the physical locations
// and directions of these quantities.
struct GeometryShader
{
public:
    virtual ~GeometryShader() { }

    // TODO: figure out a way to make this safe
    virtual void evaluate(void* mesh) { }
    virtual bool isValid(MeshType mesh_type) const { return true; }
};

// this shader interpolates the geometry of an objmesh between its actual mesh
// and a projection of its mesh onto a sphere. The sphere will have a radius
// equal to the radius of the shortest axis of its bounding box multiplied by
// the prop_of_shortest_axis parameter with its center located at the centroid
// of the mesh
struct InterpVertsToSphereShader : public GeometryShader
{
public:
    struct Params
    {
        Params(float prop, float proxy)
            : prop(prop), proxy(proxy) { }

        float prop;
        float proxy;
    };

    InterpVertsToSphereShader(float prop_of_shortest_axis, float interp);

    virtual void evaluate(void* mesh);
    virtual bool isValid(MeshType mesh_type) const;

    float prop_of_shortest_axis;
    float interp;
};

/////////////////////////////////////////////////
// Geometry Shader Node structure
/////////////////////////////////////////////////
struct GeometryShaderNode : public Node
{
public:
    GeometryShaderNode() : shader(nullptr) { }
    GeometryShaderNode(std::string name) : Node(name), shader(nullptr) { }

    ~GeometryShaderNode() { delete shader; }

    GeometryShader* shader;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Material Shader Node structure
/////////////////////////////////////////////////
struct MaterialShaderNode : public Node
{
public:
    MaterialShaderNode() : shader(nullptr) { }
    MaterialShaderNode(std::string name) : Node(name), shader(nullptr) { }

    ~MaterialShaderNode() { delete shader; }

    MaterialShader* shader;
};
/////////////////////////////////////////////////
