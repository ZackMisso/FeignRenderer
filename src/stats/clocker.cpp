/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/stats/clocker.h>

FEIGN_BEGIN()

#if CLOCKING

ClockerResults::ClockerResults()
{
    times = std::vector<Duration>(ClockerType::COUNT);
}

ClockerResults::ClockerResults(const ClockerResults &other)
{
    times = std::vector<Duration>(ClockerType::COUNT, Duration(0));

    for (int i = 0; i < ClockerType::COUNT; ++i)
    {
        times[i] = other.times[i];
    }
}

void ClockerResults::print(ClockerType tracker) const
{
    std::cout << times[tracker].count() << " seconds - " << to_name(tracker) << std::endl;
}

std::string ClockerResults::to_name(ClockerType tracker) {
    switch (tracker) {
        case API: return "api";
        case RENDERING: return "rendering";
        case CAMERA_RAY: return "camera_ray_sampling";
        case SCENE_PARSE: return "scene_parsing";
        // case SCENE_INTERSECT: return "scene";
        case INTEGRATOR: return "integrator";
        case INTEGRATOR_INTERSECT: return "integrator_intersect";
        case INTEGRATOR_PREPROCESS: return "integrator_preprocess";
        case INTEGRATOR_RR: return "integrator_rr";
        case INTEGRATOR_NEE: return "integrator_nee";
        case INTEGRATOR_EVAL: return "integrator_eval";
        case SHADER: return "shader";
        case SHADER_SURFACE: return "surface_shader";
        case SHADER_SURFACE_EVAL: return "surface_shader_eval";
        case SHADER_SURFACE_SAMPLE: return "surface_shader_sampling";
        case SHADER_SURFACE_PREPROCESS: return "surface_shader_preprocess";
        case SHADER_MEDIA: return "media_shader";
        case SHADER_MEDIA_EVAL: return "media_shader_eval";
        case SHADER_MEDIA_SAMPLE: return "media_shader_sampling";
        case SHADER_MEDIA_PREPROCESS: return "media_shader_preprocess";
        case FILTER: return "filter";
        case EMBREE: return "embree_total";
        // three enums used for debugging purposes. TODO: maybe allow custom
        // names to be set for these
        case DEBUG_ONE: return "debug_one";
        case DEBUG_TWO: return "debug_one";
        case DEBUG_THREE: return "debug_one";
        case COUNT: return "total_clockings";
        default: return "unrecognized_clocker_type";
    }
}

Clocker *Clocker::instance = nullptr;

Clocker::Clocker()
{
    clockings = ClockerResults();
    startTimes = std::vector<TimePt>(ClockerType::COUNT);
    actives = std::vector<bool>(ClockerType::COUNT, false);
}

Clocker *Clocker::getInstance() { return instance; }

void Clocker::initialize()
{
    instance = new Clocker();
}

void Clocker::deinitialize()
{
    delete instance;
}

void Clocker::printResults()
{
    getInstance()->clockings.print(API);
    getInstance()->clockings.print(RENDERING);
    getInstance()->clockings.print(CAMERA_RAY);
    getInstance()->clockings.print(SCENE_PARSE);
    getInstance()->clockings.print(INTEGRATOR);
    getInstance()->clockings.print(INTEGRATOR_INTERSECT);
    getInstance()->clockings.print(INTEGRATOR_PREPROCESS);
    getInstance()->clockings.print(INTEGRATOR_RR);
    getInstance()->clockings.print(INTEGRATOR_NEE);
    getInstance()->clockings.print(INTEGRATOR_EVAL);
    getInstance()->clockings.print(SHADER);
    getInstance()->clockings.print(SHADER_SURFACE);
    getInstance()->clockings.print(SHADER_SURFACE_EVAL);
    getInstance()->clockings.print(SHADER_SURFACE_SAMPLE);
    getInstance()->clockings.print(SHADER_SURFACE_PREPROCESS);
    getInstance()->clockings.print(SHADER_MEDIA);
    getInstance()->clockings.print(SHADER_MEDIA_EVAL);
    getInstance()->clockings.print(SHADER_MEDIA_SAMPLE);
    getInstance()->clockings.print(SHADER_MEDIA_PREPROCESS);
    getInstance()->clockings.print(FILTER);
    getInstance()->clockings.print(EMBREE);
    getInstance()->clockings.print(DEBUG_ONE);
    getInstance()->clockings.print(DEBUG_TWO);
    getInstance()->clockings.print(DEBUG_THREE);
    getInstance()->clockings.print(COUNT);
}

void Clocker::startClock(ClockerType tracker)
{
    if (getInstance()->actives[tracker])
    {
        throw new FeignRendererException(ClockerResults::to_name(tracker) + " already active");
    }

    getInstance()->actives[tracker] = true;
    getInstance()->startTimes[tracker] = Clock::now();
}

void Clocker::endClock(ClockerType tracker)
{
    if (!getInstance()->actives[tracker])
    {
        throw new FeignRendererException(ClockerResults::to_name(tracker) + " not active");
    }

    getInstance()->actives[tracker] = false;
    getInstance()->clockings.times[tracker] += Clock::now() - getInstance()->startTimes[tracker];
}

void Clocker::addClocker(ClockerType tracker)
{
    getInstance()->clockings.times[tracker] = Duration(0);
    getInstance()->startTimes[tracker] = Clock::now();
    getInstance()->actives[tracker] = false;
}

ClockerResults Clocker::returnResults()
{
    return getInstance()->clockings;
}

#endif

FEIGN_END()
