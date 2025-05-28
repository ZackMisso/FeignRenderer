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

#if RECORD
ClockerResults::ClockerResults(std::string str)
{
    times = std::vector<Duration>(ClockerType::COUNT, Duration(0));
    parse_from_string(str);
}
#endif

ClockerResults::ClockerResults(const ClockerResults &other)
{
    std::cout << "copy constructor was called" << std::endl;
    times = std::vector<Duration>(ClockerType::COUNT, Duration(0));

    for (int i = 0; i < ClockerType::COUNT; ++i)
    {
        times[i] = other.times[i];
    }
}

void ClockerResults::print_results() const
{
    int total_num_padding = 0;

    for (int i = 0; i < ClockerType::COUNT - 1; ++i)
    {
        int len = std::to_string(times[i].count()).length();
        total_num_padding = std::max(total_num_padding, len);
    }

    print(API, total_num_padding);
    print(RENDERING, total_num_padding);
    print(CAMERA_RAY, total_num_padding);
    print(SCENE_PARSE, total_num_padding);
    print(INTEGRATOR, total_num_padding);
    print(INTEGRATOR_INTERSECT, total_num_padding);
    print(INTEGRATOR_PREPROCESS, total_num_padding);
    print(INTEGRATOR_RR, total_num_padding);
    print(INTEGRATOR_NEE, total_num_padding);
    print(INTEGRATOR_EVAL, total_num_padding);
    print(SHADER, total_num_padding);
    print(SHADER_SURFACE, total_num_padding);
    print(SHADER_SURFACE_EVAL, total_num_padding);
    print(SHADER_SURFACE_SAMPLE, total_num_padding);
    print(SHADER_SURFACE_PREPROCESS, total_num_padding);
    print(SHADER_MEDIA, total_num_padding);
    print(SHADER_MEDIA_EVAL, total_num_padding);
    print(SHADER_MEDIA_SAMPLE, total_num_padding);
    print(SHADER_MEDIA_PREPROCESS, total_num_padding);
    print(FILTER, total_num_padding);
    print(EMBREE, total_num_padding);
    print(DEBUG_ONE, total_num_padding);
    print(DEBUG_TWO, total_num_padding);
    print(DEBUG_THREE, total_num_padding);

    // the total is the combination of SCENE_PARSE + API + RENDERING
    print_total();
}

#if RECORD
// TODO: make this faster
void ClockerResults::parse_from_string(std::string str) {
    std::stringstream ss(str);
    std::vector<std::string> vals = std::vector<std::string>();
    
    while (ss.good())
    {
        std::string substr;
        getline(ss, substr, ',');
        vals.push_back(substr);
    }

    month = std::stof(vals[0]);
    day = std::stof(vals[1]);
    year = std::stof(vals[2]);

    for (int i = 0; i < COUNT; ++i) {
        if (vals.size() > i) {
            times[i] = Duration(std::stof(vals[3+i]));
        } else {
            times[i] = Duration(0.0);
        }
    }

    times[COUNT] = Duration(0.0);
}

// TODO: make this faster, continuous string addition is slow
std::string ClockerResults::to_string() const {
    std::string str = "";

    str = str + std::to_string(RECORD_YEAR) + ",";
    str = str + std::to_string(RECORD_MONTH) + ",";
    str = str + std::to_string(RECORD_DAY) + ",";

    for (int i = 0; i < COUNT; ++i) {
        if (i != 0)
            str = str + ",";
        str = str + std::to_string(times[i].count());
    }

    return str;
}
#endif

void ClockerResults::print(ClockerType tracker, int padding) const
{
    std::string padded_str = std::to_string(times[tracker].count());

    // TODO: this is inefficient, fix later
    int diff = padding - padded_str.length();
    for (int i = 0; i < diff; ++i) {
        padded_str = padded_str + " ";
    }

    std::cout << padded_str << " seconds - " << to_name(tracker) << std::endl;
}

void ClockerResults::print_total() const
{
    float time = 0.f;
    
    time = times[SCENE_PARSE].count() +
           times[RENDERING].count() +
           times[API].count();

    std::cout << time << " seconds - " << to_name(COUNT) << std::endl;
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
        case DEBUG_TWO: return "debug_two";
        case DEBUG_THREE: return "debug_three";
        case COUNT: return "total_clockings";
        default: return "unrecognized_clocker_type";
    }
}

Clocker *Clocker::instance = nullptr;

Clocker::Clocker()
{
    clockings = ClockerResults();
    start_times = std::vector<TimePt>(ClockerType::COUNT);
    actives = std::vector<bool>(ClockerType::COUNT, false);
}

Clocker *Clocker::get_instance() { return instance; }

void Clocker::initialize()
{
    instance = new Clocker();
}

void Clocker::deinitialize()
{
    delete instance;
}

void Clocker::print_results()
{
    get_instance()->clockings.print_results();
}

void Clocker::start_clock(ClockerType tracker)
{
    if (get_instance()->actives[tracker])
    {
        throw new FeignRendererException(ClockerResults::to_name(tracker) + " already active");
    }

    get_instance()->actives[tracker] = true;
    get_instance()->start_times[tracker] = Clock::now();
}

void Clocker::end_clock(ClockerType tracker)
{
    if (!get_instance()->actives[tracker])
    {
        throw new FeignRendererException(ClockerResults::to_name(tracker) + " not active");
    }

    get_instance()->actives[tracker] = false;
    get_instance()->clockings.times[tracker] += Clock::now() - get_instance()->start_times[tracker];
}

void Clocker::add_clocker(ClockerType tracker)
{
    get_instance()->clockings.times[tracker] = Duration(0);
    get_instance()->start_times[tracker] = Clock::now();
    get_instance()->actives[tracker] = false;
}

void Clocker::return_results(ClockerResults* out)
{
    if (out)
        *out = get_instance()->clockings;
}

#endif

FEIGN_END()
