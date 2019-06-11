#pragma once

#include <feign/accel/accel.h>
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
