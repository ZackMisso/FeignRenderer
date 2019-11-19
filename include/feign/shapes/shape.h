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
#include <feign/math/bbox.h>
#include <feign/misc/intersection.h>
#include <feign/misc/embree_util.h>

// These structs are used for passing data to embree.
// Note: embree does not have to have normal information.
struct e_Vertex   { float x,y,z,r;  };
struct e_Triangle { int v0, v1, v2; };

class Shape : public Node
{
public:
    Shape();
    Shape(Node* parent);
    virtual ~Shape();

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const = 0;
    virtual bool intersect(uint32_t tri,
                           const Ray3f& scene_ray,
                           Intersection& its) const = 0;

    virtual void completeIntersectionInfo(const Ray3f& ray, Intersection& its) const = 0;
    virtual uint32_t primitiveCount() const = 0;

    virtual BBox3f boundingBox() const = 0;
    virtual BBox3f boundingBox(uint32_t tri) const = 0;
    virtual Point3f centroid() const = 0;
    virtual Point3f centroid(uint32_t tri) const = 0;

    virtual void addShapeToScene(RTCScene scene, RTCDevice device) = 0;

    virtual std::string getName() const;

    virtual NodeType getNodeType() const;

    Material* getMaterial() const { return material; }
    unsigned int getGeomID() const { return geomID; }

    void setMaterial(Material* param) { material = param; }

protected:
    Material* material;
    unsigned int geomID;
};
