/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <assert.h>

#include <feign/math/vector.h>
#include <feign/log/logger.h>

// these are necessary defines for imedit
#include <imedit/image.h>
#include <imedit/im_color_maps.h>

// TODO: remove this
// personal laptop
#define SCENES_PATH "/Users/corneria/Documents/Projects/FeignRenderer/scenes/"
// misc laptop
// #define SCENES_PATH "/Users/venom/Documents/Projects/FeignRenderer/scenes/"
// workstation
// #define SCENES_PATH "/home/firal/Documents/Projects/FeignRenderer/scenes/"
// work laptop
// #define SCENES_PATH "/Users/fortuna/Documents/Hobbies/FeignRenderer/scenes/"

// TODO: incorporate clocking throughout renderer
// Stats tracking //
#define CLOCKING false
////////////////////

#define Epsilon 1e-4f

#define PI           3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f

// a list of global parameters
struct GlobalParams
{
    bool ignore_shadow_checks = true;
};

static GlobalParams global_params;

// MeshTypes are used to verify that geometry shaders are valid
enum MeshType
{
    OBJ_MESH,
    SDF_MESH
};

// uncomment the following for desired types
typedef float Float;
// typedef double Float;

// TODO: do i want to use these?
typedef int Int;
// typedef long Int;

typedef uint32_t uInt;
// typedef uint64_t uInt;

// vector and matrix types
// these have no geometric meaning
typedef Vec2<Int> Vec2i;
typedef Vec2<uInt> Vec2u;
typedef Vec2<Float> Vec2f;
typedef Vec3<Int> Vec3i;
typedef Vec3<uInt> Vec3u;
typedef Vec3<Float> Vec3f;
typedef Vec4<Int> Vec4i;
typedef Vec4<uInt> Vec4u;
typedef Vec4<Float> Vec4f;

// these represent actual geometric primitives
typedef Point2<Float> Point2f;
typedef Vector3<Float> Vector3f;
typedef Point3<Float> Point3f;
typedef Normal3<Float> Normal3f;
typedef Color3<Float> Color3f;

typedef Matrix3<Float> Matrix3f;
typedef Matrix4<Float> Matrix4f;

typedef imedit::Image Imagef;

inline Float degToRad(Float value) { return value * (M_PI / 180.0); }

inline void feign_sincos(Float theta, Float *_sin, Float *_cos)
{
    *_sin = sinf(theta);
    *_cos = cosf(theta);
}

inline void LOG(std::string val)
{
    std::cout << val << std::endl;
}

inline void LOG(std::string val, int num)
{
    std::cout << val << " " << num << std::endl;
}

inline void LOG(std::string val, float num)
{
    std::cout << val << " " << num << std::endl;
}

inline void LOG(Vec3f val)
{
    std::cout << val[0] << " " << val[1] << " " << val[2] << std::endl;
}

inline void LOG(std::string val, Vec3f num)
{
    std::cout << val << " " << num[0] << " " << num[1] << " " << num[2] << std::endl;
}

inline Float bound(Float val, Float min, Float max)
{
    if (val > max) return max;
    if (val < min) return min;
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
        return 1.0f;  /* Total internal reflection! */
    }

    Float cosThetaT = std::sqrt(1.0f - sinThetaTSqr);

    Float Rs = (etaI * cos_theta - etaT * cos_theta)
             / (etaI * cos_theta + etaT * cos_theta);
    Float Rp = (etaT * cos_theta - etaI * cos_theta)
             / (etaT * cos_theta + etaI * cos_theta);

    return (Rs * Rs + Rp * Rp) / 2.0f;
}

inline Color3f color_max(Color3f one, Color3f two)
{
    return Color3f(std::max(one(0), two(0)),
                   std::max(one(1), two(1)),
                   std::max(one(2), two(2)));
}

/////////////////////////////////////////////////
// EXCEPTIONS
/////////////////////////////////////////////////
class NotImplementedException : public std::runtime_error
{
public:
    NotImplementedException() : std::runtime_error("Exception: Method Not Implemented") { }

    NotImplementedException(std::string str) : std::runtime_error("Exception: Method Not Implemented: " + str)
    {
        std::cout << str << std::endl;
    }
};

class FeignRendererException : public std::runtime_error
{
public:
    FeignRendererException() : std::runtime_error("Renderer Exception") { }

    FeignRendererException(std::string str) : std::runtime_error("Renderer Exception: " + str)
    {
        std::cout << str << std::endl;
    }
};
/////////////////////////////////////////////////
