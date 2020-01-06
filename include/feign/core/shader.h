#pragma once

#include <feign/core/node.h>

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
// Object Node structure
/////////////////////////////////////////////////
struct GeometryShaderNode : public Node
{
public:
    GeometryShaderNode() : shader(nullptr) { }
    GeometryShaderNode(std::string name) : Node(name), shader(nullptr) { }

    GeometryShader* shader;
};
/////////////////////////////////////////////////
