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

// macros for cleaning up code
#if CLOCKING
#define CLOCKER_START_ONE(start) Clocker::start_clock(start);

#define CLOCKER_STOP_ONE(stop) Clocker::end_clock(stop);

#define CLOCKER_STOP_TWO(stop, stop_2) \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2);

#define CLOCKER_STOP_THREE(stop, stop_2, stop_3) \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::end_clock(stop_3);

#define CLOCKER_START_STOP_ONE(start, stop) \
    Clocker::end_clock(stop); \
    Clocker::start_clock(start);

#define CLOCKER_START_STOP_TWO(start, start_2, stop, stop_2) \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2);

#define CLOCKER_START_STOP_THREE(start, start_2, start_3, stop, stop_2, stop_3) \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::end_clock(stop_3); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2); \
    Clocker::start_clock(start_3);

#define CLOCKER_START_TWO_STOP_ONE(start, start_2, stop) \
    Clocker::end_clock(stop); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2);

#define CLOCKER_START_ONE_STOP_TWO(start, stop, stop_2) \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::start_clock(start);

#define CLOCKER_START_ONE_STOP_THREE(start, stop, stop_2, stop_3) \
    Clocker::end_clock(stop); \
    Clocker::end_clock(stop_2); \
    Clocker::end_clock(stop_3); \
    Clocker::start_clock(start);

#define CLOCKER_START_THREE_STOP_ONE(start, start_2, start_3, stop) \
    Clocker::end_clock(stop); \
    Clocker::start_clock(start); \
    Clocker::start_clock(start_2); \
    Clocker::start_clock(start_3);

#else
// define all clocker macros as empty strings
#define CLOCKER_START_ONE(start)
#define CLOCKER_STOP_ONE(stop)
#define CLOCKER_STOP_TWO(stop, stop_2)
#define CLOCKER_STOP_THREE(stop, stop_2, stop_3)
#define CLOCKER_START_STOP_ONE(start, stop)
#define CLOCKER_START_STOP_TWO(start, start_2, stop, stop_2)
#define CLOCKER_START_STOP_THREE(start, start_2, start_3, stop, stop_2, stop_3)
#define CLOCKER_START_TWO_STOP_ONE(start, start_2, stop)
#define CLOCKER_START_ONE_STOP_TWO(start, stop, stop_2)
#define CLOCKER_START_ONE_STOP_THREE(start, stop, stop_2, stop_3)
#define CLOCKER_START_THREE_STOP_ONE(start, start_2, start_3, stop)
#endif

#if CLOCKING

#include <string>
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point TimePt;
typedef std::chrono::duration<Float> Duration;

struct ClockerResults
{
    ClockerResults();
    ClockerResults(const ClockerResults &other);

    void print_results() const;
    void print(ClockerType tracker, int padding) const;
    void print_total() const;
    static std::string to_name(ClockerType tracker);

    void operator+=(const ClockerResults& other);
    void operator/=(float val);
    void operator*=(float val);

    std::vector<Duration> times;

    #if RECORD
    ClockerResults(std::string str); // convenient for loading from file

    void parse_from_string(std::string str);
    std::string to_string() const;
    
    int month;
    int day;
    int year;
    #endif
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
