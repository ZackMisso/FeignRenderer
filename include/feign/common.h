/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <tgmath.h>
#include <assert.h>

// these are necessary defines for imedit
#include <imedit/image.h>
#include <imedit/im_color_maps.h>

#define FEIGN_BEGIN() \
    namespace feign   \
    {
#define FEIGN_END() }

// TODO: incorporate clocking throughout renderer
// Stats tracking //
#define CLOCKING true
// record should only be true during runs where we log clockings
// for weekly stat tracking
#define RECORD true
// hard coding these for now:
#define RECORD_DAY 4

#define RECORD_MONTH 4
#define RECORD_YEAR 2025
////////////////////

// TODO: make this a stats parameter in the future
// whether or not to use multithreading
#define GOTTAGOFAST false
#define OPENVDB true
#define VERBOSE false
#define NONEXPMEDIA false

// a defined parameter to remove infinite point light contributions, but induces
// bias
#define CLAMP_POINT_SINGULARITIES false
#define CLAMP_POINT_DISTANCE 0.1f

// TODO: there are some couts that do not use logs... remove them
#define OUTPUT_LOGS true

FEIGN_BEGIN()

// this is going to be used temporarily for debugging
static bool is_verbose = true;

// uncomment the following for desired types
// NOTE: need to also set imedit::Float in image.h

#define DOUBLE_PRECISION false

#define NTHREADS 12

#if DOUBLE_PRECISION
typedef double Float;
#define ZERO 0.0
#define ONE 1.0
#define TWO 2.0
#define FOUR 4.0
#define PMF_MIN 0.01
#define Epsilon 1e-3
#define EPS_VEC_X Vector3f(Epsilon * 1.0, 0.0, 0.0)
#define EPS_VEC_Y Vector3f(0.0, Epsilon * 1.0, 0.0)
#define EPS_VEC_Z Vector3f(0.0, 0.0, Epsilon * 1.0)
#define PI 3.14159265358979323846
#define INV_PI 0.31830988618379067154
#define INV_TWOPI 0.15915494309189533577
#define INV_FOURPI 0.07957747154594766788
#define SQRT_TWO 1.41421356237309504880
#define INV_SQRT_TWO 0.70710678118654752440
#else
typedef float Float;
#define ZERO 0.f
#define ONE 1.f
#define TWO 2.f
#define FOUR 4.f
#define PMF_MIN 0.01f
#define Epsilon 1e-3f
#define EPS_VEC_X Vector3f(Epsilon * 1.f, 0.f, 0.f)
#define EPS_VEC_Y Vector3f(0.f, Epsilon * 1.f, 0.f)
#define EPS_VEC_Z Vector3f(0.f, 0.f, Epsilon * 1.f)
#define PI 3.14159265358979323846f
#define INV_PI 0.31830988618379067154f
#define INV_TWOPI 0.15915494309189533577f
#define INV_FOURPI 0.07957747154594766788f
#define SQRT_TWO 1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f
#endif

#define SDF_STEPS 2000

// TODO: replace all Color3f's with just Color
// TODO: allow this to support spectrum later
#define Color Color3<Float>
// #define Color ColorX<Float>

// a list of global parameters
struct GlobalParams
{
    GlobalParams()
    {
        sdf_only = true;
        ignore_shadow_checks = false;
        name = "what";
    }

    std::string name;
    bool ignore_shadow_checks; // ignores shadow checks when determining light contribution
    bool sdf_only;             // tells the renderer whether or not it is rendering signed distance fields
};

static GlobalParams global_params = GlobalParams();

// MeshTypes are used to verify that geometry shaders are valid
enum MeshType
{
    OBJ_MESH,
    SDF_MESH
};

/////////////////////////////////////////////////

typedef imedit::Image Imagef;

inline Float degToRad(Float value)
{
    return value * (M_PI / 180.0);
}

inline void feign_sincos(Float theta, Float *_sin, Float *_cos)
{
    *_sin = sinf(theta);
    *_cos = cosf(theta);
}

// logging methods to avoid typing std::cout/std::endl everywhere
inline void LOG(std::string val)
{
#if OUTPUT_LOGS
    std::cout << val << std::endl;
#endif
}

inline std::string STR(Float val)
{
    return std::to_string(val);
}

inline std::string STR(int val)
{
    return std::to_string(val);
}

inline Float bound(Float val, Float min, Float max)
{
    if (val > max)
        return max;
    if (val < min)
        return min;
    return val;
}

// based on code from Wenzel Jakob
inline Float fresnel(Float cos_theta,
                     Float ext_ior,
                     Float int_ior)
{
    Float etaI = ext_ior;
    Float etaT = int_ior;

    if (ext_ior == int_ior)
    {
        return 0.f;
    }

    /* Swap the indices of refraction if the interaction starts
       at the inside of the object */
    if (cos_theta < 0.0f)
    {
        std::swap(etaI, etaT);
        cos_theta = -cos_theta;
    }

    /* Using Snell's law, calculate the squared sine of the
       angle between the normal and the transmitted ray */
    Float eta = etaI / etaT;
    Float sinThetaTSqr = eta * eta * (1.f - cos_theta * cos_theta);

    if (sinThetaTSqr > 1.0f)
    {
        return 1.0f; /* Total internal reflection! */
    }

    Float cosThetaT = std::sqrt(1.0f - sinThetaTSqr);

    Float Rs = (etaI * cos_theta - etaT * cos_theta) / (etaI * cos_theta + etaT * cos_theta);
    Float Rp = (etaT * cos_theta - etaI * cos_theta) / (etaT * cos_theta + etaI * cos_theta);

    return (Rs * Rs + Rp * Rp) / 2.0f;
}

inline Float interp_value(int index, int flip)
{
    if (index > flip)
    {
        return 1.f - Float(index - flip) / Float(flip);
    }

    return 1.f - Float(flip - index) / Float(flip);
}

inline Float interp(Float x, Float y, Float proxy)
{
    return x * (1.0 - proxy) + proxy * y;
}

inline Float clamp(Float proxy, Float min, Float max)
{
    if (proxy < min)
        return min;
    if (proxy > max)
        return max;
    return proxy;
}

inline Float sign(Float val)
{
    if (val < 0.0)
        return -1.0;
    return 1.0;
}

inline Float SQRT(Float val)
{
    if (val < Epsilon)
        return 0.0;
    return std::sqrt(val);
}

/////////////////////////////////////////////////
// EXCEPTIONS
/////////////////////////////////////////////////
class NotImplementedException : public std::runtime_error
{
public:
    NotImplementedException() : std::runtime_error("Exception: Method Not Implemented") {}

    NotImplementedException(std::string str) : std::runtime_error("Exception: Method Not Implemented: " + str)
    {
        std::cout << str << std::endl;
    }
};

class NotSupportedException : public std::runtime_error
{
public:
    NotSupportedException() : std::runtime_error("Exception: Feature Not Supported") {}

    NotSupportedException(std::string str) : std::runtime_error("Exception: Feature Not Supported: " + str)
    {
        std::cout << str << std::endl;
    }
};

class FeignRendererException : public std::runtime_error
{
public:
    FeignRendererException() : std::runtime_error("Renderer Exception") {}

    FeignRendererException(std::string str) : std::runtime_error("Renderer Exception: " + str)
    {
        std::cout << str << std::endl;
    }
};
/////////////////////////////////////////////////

#define CHECK_EXISTS(ptr, msg) \
    if (!ptr)                  \
        throw new FeignRendererException(msg);

FEIGN_END()
