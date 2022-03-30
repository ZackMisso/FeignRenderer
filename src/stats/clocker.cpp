/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/stats/clocker.h>

FEIGN_BEGIN()

#if CLOCKING

ClockerResults::ClockerResults()
{
    trackings = std::vector<std::string>();
    times = std::vector<Duration>();
}

ClockerResults::ClockerResults(const ClockerResults &other)
{
    trackings = std::vector<std::string>();
    times = std::vector<Duration>();

    for (int i = 0; i < other.trackings.size(); ++i)
    {
        trackings.push_back(other.trackings[i]);
        times.push_back(other.times[i]);
    }
}

void ClockerResults::print(int index)
{
    std::cout << trackings[index] << ": " << times[index].count() << " seconds" << std::endl;
}

Clocker *Clocker::instance = nullptr;

Clocker::Clocker()
{
    clockings = ClockerResults();
    startTimes = std::vector<TimePt>();
    actives = std::vector<bool>();
}

Clocker *Clocker::getInstance() { return instance; }

void Clocker::initialize()
{
    instance = new Clocker();

    Clocker::addClocker("render");
    Clocker::addClocker("parse");
    Clocker::addClocker("scene intersect");
    Clocker::addClocker("integrator");
    Clocker::addClocker("shader eval");
    Clocker::addClocker("filter");
    Clocker::addClocker("embree");

    // add more soon
}

void Clocker::deinitialize()
{
    delete instance;
}

void Clocker::printResults()
{
    for (int i = 0; i < getInstance()->clockings.trackings.size(); ++i)
    {
        getInstance()->clockings.print(i);
    }
}

void Clocker::startClock(std::string tracker)
{
    // TODO: replace strings with hashes for more efficiency
    for (int i = 0; i < getInstance()->clockings.trackings.size(); ++i)
    {
        if (getInstance()->clockings.trackings[i] == tracker)
        {
            if (getInstance()->actives[i])
            {
                throw new FeignRendererException(tracker + " already active");
            }

            getInstance()->actives[i] = true;
            getInstance()->startTimes[i] = Clock::now();

            i = getInstance()->clockings.trackings.size();
        }
    }
}

void Clocker::endClock(std::string tracker)
{
    // TODO: replace strings with hashes for more efficiency
    for (int i = 0; i < getInstance()->clockings.trackings.size(); ++i)
    {
        if (getInstance()->clockings.trackings[i] == tracker)
        {
            if (!getInstance()->actives[i])
            {
                throw new FeignRendererException(tracker + " not active");
            }

            getInstance()->actives[i] = false;
            getInstance()->clockings.times[i] += Clock::now() - getInstance()->startTimes[i];

            i = getInstance()->clockings.trackings.size();
        }
    }
}

void Clocker::addClocker(std::string tracker)
{
    getInstance()->clockings.trackings.push_back(tracker);
    getInstance()->clockings.times.push_back(Duration(0));
    getInstance()->startTimes.push_back(Clock::now());
    getInstance()->actives.push_back(false);
}

ClockerResults Clocker::returnResults()
{
    return getInstance()->clockings;
}

#endif

FEIGN_END()
