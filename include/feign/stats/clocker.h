/**
 * Author:    Zackary Misso
 * Version:   0.2.0
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

struct ClockerResults
{
    ClockerResult()
    {
        trackings = std::vector<std::string>();
        times = std::vector<Duration>();
    }

    ClockerResult(const ClockerResult &other)
    {
        // TODO
    }

    void print(int index)
    {
        std::cout << trackings[i] << ": " << durations[i].count() << " seconds" << std::endl;
    }

    std::vector<std::string> trackings;
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
    static void startClock(std::string tracker);
    static void endClock(std::string tracker);
    static void addClocker(std::string tracker);
    static ClockerResult returnResult(std::string tracking);
    static void returnResults(std::vector<ClockerResult> &results);
};

#endif

FEIGN_END()
