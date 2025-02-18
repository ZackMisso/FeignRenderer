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
    times = std::vector<Duration>(ClockerType::COUNT, Duration(0));
}

ClockerResults::ClockerResults(const ClockerResults &other)
{
    times = std::vector<Duration>(ClockerType::COUNT, Duration(0));

    for (int i = 0; i < ClockerType::COUNT; ++i)
    {
        times[i] = other.times[i];
    }
}

void ClockerResults::print_results() const
{
    print(API);
    print(RENDERING);
    print(CAMERA_RAY);
    print(SCENE_PARSE);
    print(INTEGRATOR);
    print(INTEGRATOR_INTERSECT);
    print(INTEGRATOR_PREPROCESS);
    print(INTEGRATOR_RR);
    print(INTEGRATOR_NEE);
    print(INTEGRATOR_EVAL);
    print(SHADER);
    print(SHADER_SURFACE);
    print(SHADER_SURFACE_EVAL);
    print(SHADER_SURFACE_SAMPLE);
    print(SHADER_SURFACE_PREPROCESS);
    print(SHADER_MEDIA);
    print(SHADER_MEDIA_EVAL);
    print(SHADER_MEDIA_SAMPLE);
    print(SHADER_MEDIA_PREPROCESS);
    print(FILTER);
    print(EMBREE);
    print(DEBUG_ONE);
    print(DEBUG_TWO);
    print(DEBUG_THREE);
    print(COUNT);
}

void ClockerResults::print(ClockerType tracker) const
{
    std::cout << times[tracker].count() << " seconds - " << to_name(tracker) << std::endl;
}

std::string ClockerResults::to_name(ClockerType tracker)
{
    switch (tracker)
    {
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
    getInstance()->clockings.print_results();
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

void Clocker::returnResults(ClockerResults* out)
{
    if (out)
        *out = getInstance()->clockings;
}

#endif

FEIGN_END()
