/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

FEIGN_BEGIN()

#if CLOCKING

#include <string>
#include <chrono>
#include <string>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point TimePt;
typedef std::chrono::duration<Float> Duration;

// a list of enums used to quickly reference which clocker is being used
enum ClockerType {
    API,
    RENDERING,
    CAMERA_RAY,
    SCENE_PARSE,
    // SCENE_INTERSECT, // maybe remove this one
    INTEGRATOR,
    INTEGRATOR_INTERSECT,
    INTEGRATOR_PREPROCESS,
    INTEGRATOR_RR,
    INTEGRATOR_NEE,
    INTEGRATOR_EVAL,
    SHADER,
    SHADER_SURFACE,
    SHADER_SURFACE_EVAL,
    SHADER_SURFACE_SAMPLE,
    SHADER_SURFACE_PREPROCESS,
    SHADER_MEDIA,
    SHADER_MEDIA_EVAL,
    SHADER_MEDIA_SAMPLE,
    SHADER_MEDIA_PREPROCESS,
    // TODO: add emitter sampling clockers
    // TODO: add ff sampling clockers
    // TODO: add transmittance estimation clockers
    FILTER,
    EMBREE,
    // three enums used for debugging purposes
    DEBUG_ONE,
    DEBUG_TWO,
    DEBUG_THREE,
    COUNT
};

struct ClockerResults
{
    ClockerResults();
    ClockerResults(const ClockerResults &other);

    void print(ClockerType tracker) const;
    static std::string to_name(ClockerType tracker);

    std::vector<Duration> times;
};

// TODO: need to implement this with support for multi-threading
class Clocker
{
private:
    static Clocker *instance;

    Clocker();

    ClockerResults clockings;
    std::vector<TimePt> startTimes;
    std::vector<bool> actives;

public:
    static Clocker *getInstance();
    static void initialize();
    static void deinitialize();
    static void printResults();
    static void startClock(ClockerType tracker);
    static void endClock(ClockerType tracker);
    static void addClocker(ClockerType tracker);
    static ClockerResults returnResults();
};

#endif

FEIGN_END()
