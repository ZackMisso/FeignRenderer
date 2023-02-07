/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/shader.h>
#include <feign/media/media.h>
#include <feign/math/bbox.h>
#include <feign/math/transform.h>
#include <feign/misc/intersection.h>
#include <feign/misc/embree_util.h>
#include <feign/misc/medium_boundry.h>

FEIGN_BEGIN()

class EmitterNode;

// These structs are used for passing data to embree.
// Note: embree does not have to have normal information.
struct e_Vertex
{
    float x, y, z, r;
};
struct e_Triangle
{
    int v0, v1, v2;
};

/////////////////////////////////////////////////
// Shape abstraction
/////////////////////////////////////////////////
class Shape
{
public:
    // Shape();
    Shape(const MediumBoundry *boundry, bool is_null);
    virtual ~Shape() {}

    virtual bool intersect(const Ray3f &scene_ray, Intersection &its) const
    {
        return false;
    }

    virtual void completeIntersectionInfo(Intersection &its) const {}
    virtual uint32_t primitiveCount() const = 0;

    virtual BBox3f boundingBox() const = 0;
    virtual Point3f centroid() const = 0;

    virtual Float surface_area(int primitive) const { return 0.0; };

    virtual void preProcess(bool requires_processing = false) {}

    // this is only used by embree
    virtual void addShapeToScene(RTCScene scene, RTCDevice device) {}

    unsigned int getGeomID() const { return geomID; }
    unsigned int getInstID() const { return instID; }

    void setInstID(unsigned int val) { instID = val; }
    void setGeomID(unsigned int val) { geomID = val; }

    Transform transform;
    GeometryShaderNode *geomShader;
    const MediumBoundry *boundry;

    bool is_null;

protected:
    unsigned int geomID;
    unsigned int instID;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Mesh Node structure
/////////////////////////////////////////////////
struct MeshNode : public Node
{
public:
    MeshNode() : mesh(nullptr) {}
    MeshNode(Shape *mesh) : mesh(mesh) {}
    MeshNode(std::string name) : Node(name), mesh(nullptr) {}

    ~MeshNode() { delete mesh; }

    Shape *mesh;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Object Node structure
/////////////////////////////////////////////////
struct ObjectNode : public Node
{
public:
    ObjectNode() : mesh(nullptr),
                   emitter(nullptr) {}
    ObjectNode(MeshNode *mesh) : mesh(mesh),
                                 emitter(nullptr) {}
    ObjectNode(std::string name) : Node(name),
                                   mesh(nullptr),
                                   emitter(nullptr) {}

    ~ObjectNode() {}

    MeshNode *mesh;
    MaterialShaderNode *material_shader;
    EmitterNode *emitter;
    // TODO: I need to rething this design... should media be a parameter of
    //       shape or just exist in the object node
    // MediaNode* medium;
    Transform transform;
};
/////////////////////////////////////////////////

FEIGN_END()
