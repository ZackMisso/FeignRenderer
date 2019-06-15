#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <feign/exceptions.h>
#include <feign/math/vector.h>
#include <feign/log/logger.h>

// these are necessary defines for imedit
#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_STATIC

#include <imedit/image.h>

// TODO: remove this
// personal laptop
// #define SCENES_PATH "/Users/corneria/Documents/Projects/FeignRenderer/scenes/"
// misc laptop
// #define SCENES_PATH "/Users/venom/Documents/Projects/FeignRenderer/scenes/"
// workstation
#define SCENES_PATH "/home/firal/Documents/Projects/FeignRenderer/scenes/"
// work laptop
// #define SCENES_PATH "/Users/fortuna/Documents/Hobbies/FeignRenderer/scenes/"

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

// uncomment the following for desired types
typedef float Float;
// typedef double Float;

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

typedef imedit::Image<Float> Imagef;
typedef imedit::Image<uInt> Imagei;

inline Float degToRad(Float value) { return value * (M_PI / 180.0); }

// maybe move this out of common
#if !defined(_GNU_SOURCE)
    /// Emulate sincosf using sinf() and cosf()
    inline void sincosf(Float theta, Float *_sin, Float *_cos) {
        *_sin = sinf(theta);
        *_cos = cosf(theta);
    }
#endif
