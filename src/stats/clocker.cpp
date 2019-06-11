#include <feign/stats/clocker.h>

#if CLOCKING

Clocker::Clocker()
{
    trackings = std::vector<std::string>();
    startTimes = std::vector<Clock>();
    durations = std::vector<long>();
    actives = std::vector<bool>();
}

Clocker* Clocker::getInstance() { return instance; }

void Clocker::initialize()
{
    instance = new Clocker();

    addClocker("full render");
    addClocker("embree intersect");
    // add more soon
}

void Clocker::deinitialize()
{
    delete instance;
}

void Clocker::printResults()
{
    // TODO
}

void Clocker::startClock(std::string tracker)
{
    // TODO: replace strings with hashes for more efficiency
    for (int i = 0; i < trackings.size(); ++i)
    {
        if (trackings[i] == tracker)
        {
            if (actives[i])
            {
                throw new ClockerException(tracker + " already active");
            }

            actives[i] = true;
            startTimes[i] = Clock::now();

            i = trackings.size();
        }
    }
}

void Clocker::endClock(std::string tracker)
{
    // TODO
}

void Clocker::addClocker(std::string tracker)
{
    trackings.push_back(tracker);
    startTimes.push_back(Clock::now());
    durations.push_back(0);
    actives.push_back(false);
}

long Clocker::getClockerDuration(std::string tracker)
{
    // TODO
    return -1;
}

#endif
