#pragma once

#include <feign/node.h>
#include <feign/math/bbox.h>
#include <feign/misc/intersection.h>
#include <feign/misc/embree_util.h>
#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>
#include <embree3/rtcore_scene.h>

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

    virtual void preProcess() = 0;
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

    unsigned int getGeomID() const { return geomID; }

protected:
    unsigned int geomID;
};
