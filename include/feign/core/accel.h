/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/math/ray.h>
#include <feign/shapes/shape.h>
#include <feign/misc/intersection.h>

/////////////////////////////////////////////////
// Ray Tracing Acceleration Structure
/////////////////////////////////////////////////
class Accel : public Node
{
public:
    Accel();
    ~Accel();

    virtual void clear() = 0;
    virtual void build() = 0;
    virtual void addShape(Shape* mesh) = 0;

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const = 0;

    virtual std::string getName() const;
    virtual NodeType getNodeType() const;

    void setMeshes(const std::vector<Shape*>& param);

protected:
    std::vector<Shape*> meshes;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Embree Acceleration Structure
/////////////////////////////////////////////////
#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>
#include <embree3/rtcore_scene.h>

class EmbreeAccel : public Accel
{
public:
    virtual void preProcess();
    virtual void clear();
    virtual void addShape(Shape* mesh);
    virtual void build();
    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;

    static void embree_error_handler(void* userPtr, const RTCError code, const char* str)
    {
        if (code == RTC_ERROR_NONE)
            return;

        printf("Embree: ");
        switch (code)
        {
            case RTC_ERROR_UNKNOWN          : printf("RTC_ERROR_UNKNOWN"); break;
            case RTC_ERROR_INVALID_ARGUMENT : printf("RTC_ERROR_INVALID_ARGUMENT"); break;
            case RTC_ERROR_INVALID_OPERATION: printf("RTC_ERROR_INVALID_OPERATION"); break;
            case RTC_ERROR_OUT_OF_MEMORY    : printf("RTC_ERROR_OUT_OF_MEMORY"); break;
            case RTC_ERROR_UNSUPPORTED_CPU  : printf("RTC_ERROR_UNSUPPORTED_CPU"); break;
            case RTC_ERROR_CANCELLED        : printf("RTC_ERROR_CANCELLED"); break;
            default                         : printf("invalid error code"); break;
        }
        if (str)
        {
            printf(" (");
            while (*str) putchar(*str++);
            printf(")\n");
        }
        exit(1);
    }

protected:
    RTCScene scene = nullptr;
    RTCDevice device = nullptr;
    unsigned userGeomId;
    std::string rtcore; // configuration
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Naive Acceleration Structure
/////////////////////////////////////////////////
class NaiveAccel : public Accel
{
    virtual void preProcess();
    virtual void clear();
    virtual void addShape(Shape* mesh);
    virtual void build();
    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;
};
/////////////////////////////////////////////////