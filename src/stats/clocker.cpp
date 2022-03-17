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

Clocker *Clocker::instance = nullptr;

Clocker::Clocker()
{
    trackings = std::vector<std::string>();
    startTimes = std::vector<TimePt>();
    durations = std::vector<Duration>();
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
    for (int i = 0; i < getInstance()->trackings.size(); ++i)
    {
        std::cout << getInstance()->trackings[i] << ": " << getInstance()->durations[i].count() << " seconds" << std::endl;
    }
}

void Clocker::startClock(std::string tracker)
{
    // TODO: replace strings with hashes for more efficiency
    for (int i = 0; i < getInstance()->trackings.size(); ++i)
    {
        if (getInstance()->trackings[i] == tracker)
        {
            if (getInstance()->actives[i])
            {
                throw new FeignRendererException(tracker + " already active");
            }

            getInstance()->actives[i] = true;
            getInstance()->startTimes[i] = Clock::now();

            i = getInstance()->trackings.size();
        }
    }
}

void Clocker::endClock(std::string tracker)
{
    // TODO: replace strings with hashes for more efficiency
    for (int i = 0; i < getInstance()->trackings.size(); ++i)
    {
        if (getInstance()->trackings[i] == tracker)
        {
            if (!getInstance()->actives[i])
            {
                throw new FeignRendererException(tracker + " not active");
            }

            getInstance()->actives[i] = false;
            getInstance()->durations[i] += Clock::now() - getInstance()->startTimes[i];

            i = getInstance()->trackings.size();
        }
    }
}

void Clocker::addClocker(std::string tracker)
{
    getInstance()->trackings.push_back(tracker);
    getInstance()->startTimes.push_back(Clock::now());
    getInstance()->durations.push_back(Duration(0));
    getInstance()->actives.push_back(false);
}

long Clocker::getClockerDuration(std::string tracker)
{
    throw new NotImplementedException("clocker getClockerDuration");
    return -1;
}

#endif

FEIGN_END()
