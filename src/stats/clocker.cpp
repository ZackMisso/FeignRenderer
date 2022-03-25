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
    for (int i = 0; i < getInstance()->trackings.size(); ++i)
    {
        getInstance()->clockings->print(i);
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

void Clocker::returnResults(std::vector<ClockerResult> &results)
{
    for (int i = 0; i < getInstance()->trackings.size(); ++i)
    {
        results.push_back(ClockerResult(getInstance()->trackings[i], getInstance()->durations[i]));
    }
}

ClockerResult Clocker::returnResult(std::string tracking)
{
    for (int i = 0; i < getInstance()->trackings.size(); ++i)
    {
        if (getInstance()->trackings[i] == tracking)
        {
            return ClockerResult(tracking, getInstance()->durations[i]);
        }
    }
}

#endif

FEIGN_END()
