/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/shape.h>
#include <feign/core/sdf.h>
#include <feign/math/ray.h>
#include <feign/misc/intersection.h>

#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>
#include <embree3/rtcore_scene.h>

FEIGN_BEGIN()

/////////////////////////////////////////////////
// Ray Tracing Acceleration Structure
/////////////////////////////////////////////////
class RayAccel
{
public:
    virtual ~RayAccel() {}

    virtual void clear() = 0;
    virtual void build() = 0;
    virtual void preProcess() = 0;
    virtual void addShape(Shape *mesh){};
    virtual void addSDFShape(SDFShape *mesh){};

    virtual bool intersect(const Ray3f &scene_ray, Intersection &its) const = 0;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Embree Acceleration Structure
/////////////////////////////////////////////////
class EmbreeAccel : public RayAccel
{
public:
    EmbreeAccel();
    ~EmbreeAccel();

    virtual void preProcess();
    virtual void clear();
    virtual void addShape(Shape *mesh);
    virtual void build();
    virtual bool intersect(const Ray3f &scene_ray, Intersection &its) const;

    static void embree_error_handler(void *userPtr, const RTCError code, const char *str)
    {
        if (code == RTC_ERROR_NONE)
            return;

        printf("Embree: ");
        switch (code)
        {
        case RTC_ERROR_UNKNOWN:
            printf("RTC_ERROR_UNKNOWN");
            break;
        case RTC_ERROR_INVALID_ARGUMENT:
            printf("RTC_ERROR_INVALID_ARGUMENT");
            break;
        case RTC_ERROR_INVALID_OPERATION:
            printf("RTC_ERROR_INVALID_OPERATION");
            break;
        case RTC_ERROR_OUT_OF_MEMORY:
            printf("RTC_ERROR_OUT_OF_MEMORY");
            break;
        case RTC_ERROR_UNSUPPORTED_CPU:
            printf("RTC_ERROR_UNSUPPORTED_CPU");
            break;
        case RTC_ERROR_CANCELLED:
            printf("RTC_ERROR_CANCELLED");
            break;
        default:
            printf("invalid error code");
            break;
        }
        if (str)
        {
            printf(" (");
            while (*str)
                putchar(*str++);
            printf(")\n");
        }
        exit(1);
    }

protected:
    std::vector<Shape *> meshes;
    RTCScene scene = nullptr;
    RTCDevice device = nullptr;
    unsigned userGeomId;
    std::string rtcore; // configuration
};
/////////////////////////////////////////////////

// TODO: make a combined embree + sdf acceleration structure
/////////////////////////////////////////////////
// SDF Acceleration Structure
/////////////////////////////////////////////////
// This acceleration structure is used in scenes where only sign distance
// functions exist
/////////////////////////////////////////////////
class SDFAccel : public RayAccel
{
    virtual void preProcess();
    virtual void clear();
    virtual void addSDFShape(SDFShape *mesh);
    virtual void build();
    virtual bool intersect(const Ray3f &scene_ray, Intersection &its) const;

protected:
    Float sd_evaluate(Point3f point, int &shape_index) const;
    Float sd_smooth_union(Float d1, Float d2, Float k) const;
    Normal3f sd_normal(Point3f point) const;

    std::vector<SDFShape *> sdfs;
    BBox3f scene_box;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Accel Node structure
/////////////////////////////////////////////////
// TODO: actually use this somewhere
struct RayAccelNode : public Node
{
public:
    RayAccelNode() : accel(nullptr) {}
    RayAccelNode(std::string name) : Node(name), accel(nullptr) {}
    RayAccelNode(RayAccel *accel) : accel(accel) {}

    RayAccel *accel;
};
/////////////////////////////////////////////////

FEIGN_END()
