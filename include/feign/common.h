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

// these are necessary defines for imedit
#include <imedit/image.h>
#include <imedit/im_color_maps.h>

// TODO: remove this so everyone forgets im a star fox fanboy
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

// TODO: make this a stats parameter in the future
// whether or not to use multithreading
#define GOTTAGOFAST true

#define OPENVDB true

#define Epsilon 1e-4f
#define EPS_VEC_X Vector3f(Epsilon * 100.f, 0.f, 0.f)
#define EPS_VEC_Y Vector3f(0.f, Epsilon * 100.f, 0.f)
#define EPS_VEC_Z Vector3f(0.f, 0.f, Epsilon * 100.f)
#define SDF_STEPS 2000

#define PI           3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f

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
// VECTOR TYPES
/////////////////////////////////////////////////
template <typename T>
struct Point2 : public Vec2<T>
{
    Point2() : Vec2<T>() { }
    Point2(const Vec2<T>& other) : Vec2<T>(other) { }
    Point2(T c) : Vec2<T>(c) { }
    Point2(T x, T y) : Vec2<T>(x, y) { }
};

template <typename T>
struct Vector3 : public Vec3<T>
{
    Vector3() : Vec3<T>() { }
    Vector3(const Vec3<T>& vec) : Vec3<T>(vec) { }
    Vector3(T c) : Vec3<T>(c) { }
    Vector3(T x, T y, T z) : Vec3<T>(x, y, z) { }
    Vector3(Vec2<T> xy, T z) : Vec3<T>(xy, z) { }
};

template <typename T>
struct Point3 : public Vec3<T>
{
    Point3() : Vec3<T>() { }
    Point3(const Vec3<T>& vec) : Vec3<T>(vec) { }
    Point3(T c) : Vec3<T>(c) { }
    Point3(T x, T y, T z) : Vec3<T>(x, y, z) { }
    Point3(Vec2<T> xy, T z) : Vec3<T>(xy, z) { }
};

template <typename T>
struct Normal3 : public Vec3<T>
{
    Normal3() : Vec3<T>() { }
    Normal3(const Vec3<T>& vec) : Vec3<T>(vec) { }
    Normal3(T c) : Vec3<T>(c) { }
    Normal3(T x, T y, T z) : Vec3<T>(x, y, z) { }
    Normal3(Vec2<T> xy, T z) : Vec3<T>(xy, z) { }
};

template <typename T>
struct Color3 : public Vec3<T>
{
    Color3() : Vec3<T>() { }
    Color3(const Vec3<T>& vec) : Vec3<T>(vec) { }
    Color3(T c) : Vec3<T>(c) { }
    Color3(T x, T y, T z) : Vec3<T>(x, y, z) { }
    Color3(Vec2<T> xy, T z) : Vec3<T>(xy, z) { }

    bool is_black() const
    {
        return std::abs(Vec3<T>::operator()(0)) < Epsilon &&
               std::abs(Vec3<T>::operator()(1)) < Epsilon &&
               std::abs(Vec3<T>::operator()(2)) < Epsilon;
    }
};

template <typename T>
struct Vector4 : public Vec4<T>
{
    Vector4() : Vec4<T>() { }
    Vector4(const Vec4<T>& vec) : Vec4<T>(vec) { }
    Vector4(T c) : Vec4<T>(c) { }
    Vector4(T x, T y, T z, T w) : Vec4<T>(x, y, z, w) { }
    Vector4(Vec2<T> xy, T z, T w) : Vec4<T>(xy, z, w) { }
    Vector4(Vec2<T> xy, Vec2<T>zw) : Vec4<T>(xy, zw) { }
    Vector4(Vec3<T> xyz, T w) : Vec4<T>(xyz, w) { }
};
/////////////////////////////////////////////////

// uncomment the following for desired types
typedef float Float;
// typedef double Float;

// vector and matrix types
// these have no geometric meaning
typedef Vec2<int> Vec2i;
typedef Vec2<uint32_t> Vec2u;
typedef Vec2<Float> Vec2f;
typedef Vec3<int> Vec3i;
typedef Vec3<uint32_t> Vec3u;
typedef Vec3<Float> Vec3f;
typedef Vec4<int> Vec4i;
typedef Vec4<uint32_t> Vec4u;
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

struct EmitterEval
{
    Color3f throughput;
    Vector3f shadow_ray;
    bool valid = false;
};

#define COLOR_BLACK Color3f(0.f)

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

inline void LOG(Vec2f val)
{
    std::cout << val[0] << " " << val[1] << " " << std::endl;
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

inline float interp_value(int index, int flip)
{
    if (index > flip)
    {
        return 1.f - float(index - flip) / float(flip);
    }

    return 1.f - float(flip - index) / float(flip);
}

inline Float interp(Float x, Float y, Float proxy)
{
    return x * (1.0 - proxy) + proxy * y;
}

inline Float clamp(Float proxy, Float min, Float max)
{
    if (proxy < min) return min;
    if (proxy > max) return max;
    return proxy;
}

inline Float sign(Float val)
{
    if (val < 0.0) return -1.0;
    return 1.0;
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
