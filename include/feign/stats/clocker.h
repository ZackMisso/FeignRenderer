#pragma once

#if CLOCKING

#include <string>
#include <chrono>
#include <string>

#typedef std::chrono::highresolution_clock::time_point Clock;

class Clocker
{
private:
    Clocker* instance = nullptr;

    Clocker();

    std::vector<std::string> trackings;
    std::vector<Clock> startTimes;
    std::vector<long> durations;
    std::vector<bool> actives;
public:
    static Clocker* getInstance();
    static void initialize();
    static void deinitialize();
    static void printResults();
    static void startClock(std::string tracker);
    static void endClock(std::string tracker);
    static void addClocker(std::string tracker);
    static long getClockerDuration(std::string tracker);
}

#endif