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
#include <feign/core/shader.h>
#include <feign/media/media.h>
#include <feign/math/bbox.h>
#include <feign/math/transform.h>
#include <feign/misc/intersection.h>
#include <feign/misc/embree_util.h>

class EmitterNode;

// These structs are used for passing data to embree.
// Note: embree does not have to have normal information.
struct e_Vertex   { float x,y,z,r;  };
struct e_Triangle { int v0, v1, v2; };

/////////////////////////////////////////////////
// Shape abstraction
/////////////////////////////////////////////////
class Shape
{
public:
    Shape();
    virtual ~Shape() { }

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const
    {
        return false;
    }

    virtual void completeIntersectionInfo(Intersection& its) const { }
    virtual uint32_t primitiveCount() const = 0;

    virtual BBox3f boundingBox() const = 0;
    virtual Point3f centroid() const = 0;

    virtual Float surface_area(int primitive) const { return 0.0; };

    virtual void preProcess(bool requires_processing = false) { }

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

/////////////////////////////////////////////////
// Signed Distance Function Shape
/////////////////////////////////////////////////
class SDFShape : public Shape
{
public:
    SDFShape() { }
    ~SDFShape() { }

    virtual float evaluate(Point3f pt) const = 0;

    Float interp;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Sphere shape
/////////////////////////////////////////////////
class SDFSphere : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f center,
               Float radius,
               Float interp)
            : center(center),
              radius(radius),
              interp(interp) { }

        Point3f center;
        Float radius;
        Float interp;
    };

    SDFSphere(Point3f center,
              Float radius,
              Float interp);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f center;
    Float radius;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Plane shape
/////////////////////////////////////////////////
class SDFPlane : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f center,
               Normal3f normal,
               Float interp)
            : center(center),
              normal(normal),
              interp(interp) { }

        Point3f center;
        Normal3f normal;
        Float interp;
    };

    SDFPlane(Point3f center,
             Normal3f normal,
             Float interp);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f center;
    Normal3f normal;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Box shape
/////////////////////////////////////////////////
class SDFBox : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f tlc, Point3f brc)
            : tlc(tlc), brc(brc) { }

        Point3f tlc;
        Point3f brc;
    };

    SDFBox(Point3f tlc, Point3f brc);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f tlc;
    Point3f brc;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Cylinder shape
/////////////////////////////////////////////////
class SDFCylinder : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f first,
               Point3f second,
               float radius)
            : first(first),
              second(second),
              radius(radius) { }

        Point3f first;
        Point3f second;
        float radius;
    };

    SDFCylinder(Point3f first,
                Point3f second,
                float radius);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f first;
    Point3f second;
    float radius;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Signed Distance Function Cone shape
/////////////////////////////////////////////////
class SDFCone : public SDFShape
{
public:
    struct Params
    {
        Params(Point3f first,
               Point3f second,
               Float radius_1,
               Float radius_2)
            : first(first),
              second(second),
              radius_1(radius_1),
              radius_2(radius_2) { }

        Point3f first;
        Point3f second;
        Float radius_1;
        Float radius_2;
    };

    SDFCone(Point3f first,
            Point3f second,
            Float radius_1,
            Float radius_2);

    virtual Float evaluate(Point3f pt) const;

    virtual BBox3f boundingBox() const;
    virtual Point3f centroid() const;

    virtual uint32_t primitiveCount() const { return 1; }

protected:
    Point3f first;
    Point3f second;
    Float radius_1;
    Float radius_2;
};
/////////////////////////////////////////////////

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
    ObjectNode() : mesh(nullptr),
                   emitter(nullptr),
                   medium(nullptr) { }
    ObjectNode(MeshNode* mesh) : mesh(mesh),
                                 emitter(nullptr),
                                 medium(nullptr) { }
    ObjectNode(std::string name) : Node(name),
                                   mesh(nullptr),
                                   emitter(nullptr),
                                   medium(nullptr) { }

    ~ObjectNode() { }

    MeshNode* mesh;
    MaterialShaderNode* material_shader;
    EmitterNode* emitter;
    MediaNode* medium;
    Transform transform;
};
/////////////////////////////////////////////////
