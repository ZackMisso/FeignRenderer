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

// macros for cleaning up code
#define CLOCKER_START_ONE(start) \
#if CLOCKING \
    Clocker::start_clock(start); \
#endif

#define CLOCKER_STOP_ONE(start) \
#if CLOCKING \
    Clocker::stop_clock(start); \
#endif

#define CLOCKER_START_STOP_ONE(start, stop) \
#if CLOCKING \
    Clocker::end_clock(stop); \
    Clocker::start_clock(start); \
#endif

#define CLOCKER_START_STOP_TWO(start, start_2, stop, stop_2) \
#if CLOCKING \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2); \
#endif

#define CLOCKER_START_STOP_THREE(start,
                                 start_2,
                                 start_3,
                                 stop,
                                 stop_2,
                                 stop_3) \
#if CLOCKING \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::end_clock(stop_3); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2); \
    Clocker::start_clock(start_3); \
#endif

#define CLOCKER_START_TWO_STOP_ONE(start, start_2, stop) \
#if CLOCKING \
    Clocker::end_clock(stop); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2); \
#endif

#define CLOCKER_START_ONE_STOP_TWO(start, stop, stop_2) \
#if CLOCKING \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::start_clock(start); \
#endif

#define CLOCKER_START_ONE_STOP_THREE(start,
                                     stop,
                                     stop_2,
                                     stop_3) \
#if CLOCKING \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::end_clock(stop_3); \
    Clocker::start_clock(start); \
#endif

#define CLOCKER_START_THREE_STOP_ONE(start,
                                     start_2,
                                     start_3,
                                     stop) \
#if CLOCKING \
    Clocker::end_clock(stop); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2); \
    Clocker::start_clock(start_3); \
#endif

// #define CLOCKER_START_ONE_STOP_TWO(one, two) = // TODO

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

    void print_results() const;
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
    std::vector<TimePt> start_times;
    std::vector<bool> actives;

public:
    static Clocker *get_instance();
    static void initialize();
    static void deinitialize();
    static void print_results();
    static void start_clock(ClockerType tracker);
    static void end_clock(ClockerType tracker);
    static void add_clocker(ClockerType tracker);
    static void return_results(ClockerResults* out);
};

#endif

FEIGN_END()
