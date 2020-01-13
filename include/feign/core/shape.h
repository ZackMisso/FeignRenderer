/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/material.h>
#include <feign/core/shader.h>
#include <feign/math/bbox.h>
#include <feign/math/transform.h>
#include <feign/misc/intersection.h>
#include <feign/misc/embree_util.h>

// These structs are used for passing data to embree.
// Note: embree does not have to have normal information.
struct e_Vertex   { float x,y,z,r;  };
struct e_Triangle { int v0, v1, v2; };

class Shape
{
public:
    Shape();
    virtual ~Shape() { }

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const
    {
        return false;
    }

    virtual bool intersect(uint32_t tri,
                           const Ray3f& scene_ray,
                           Intersection& its) const
    {
       return false;
    }

    virtual void completeIntersectionInfo(const Ray3f& ray, Intersection& its) const = 0;
    virtual uint32_t primitiveCount() const = 0;

    virtual BBox3f boundingBox() const = 0;
    virtual BBox3f boundingBox(uint32_t tri) const = 0;
    virtual Point3f centroid() const = 0;
    virtual Point3f centroid(uint32_t tri) const = 0;

    virtual void preProcess() { }

    // this is only used by embree
    virtual void addShapeToScene(RTCScene scene, RTCDevice device) { }

    unsigned int getGeomID() const { return geomID; }
    unsigned int getInstID() const { return instID; }

    void setInstID(unsigned int val) { instID = val; }

    Transform transform;

    GeometryShaderNode* geomShader;
protected:
    unsigned int geomID;
    unsigned int instID;
};

/////////////////////////////////////////////////
// Mesh Node structure
/////////////////////////////////////////////////
struct MeshNode : public Node
{
public:
    MeshNode() : mesh(nullptr) { }
    MeshNode(Shape* mesh) : mesh(mesh) { }
    MeshNode(std::string name) : Node(name), mesh(nullptr) { }

    ~MeshNode() { delete mesh; }

    Shape* mesh;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Object Node structure
/////////////////////////////////////////////////
struct ObjectNode : public Node
{
public:
    ObjectNode() : mesh(nullptr) { }
    ObjectNode(MeshNode* mesh) : mesh(mesh) { }
    ObjectNode(std::string name) : Node(name), mesh(nullptr) { }

    ~ObjectNode() { }

    MeshNode* mesh;
    MaterialNode* material;
    Transform transform;
};
/////////////////////////////////////////////////
