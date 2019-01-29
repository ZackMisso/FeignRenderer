#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <feign/exceptions.h>
#include <feign/math/vector.h>
#include <feign/math/bbox.h>
#include <feign/log/logger.h>

// these are necessary defines for imedit
#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_STATIC

#include <imedit/image.h>

// TODO: remove this
// main laptop
// #define SCENES_PATH "/Users/corneria/Documents/Projects/FeignRenderer/scenes/"
// work laptop
// #define SCENES_PATH "/Users/venom/Documents/Projects/FeignRenderer/scenes/"
// workstation
#define SCENES_PATH "/home/firal/Documents/Projects/FeignRenderer/scenes/"

#define Epsilon 1e-4f

#define PI           3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f

typedef float Float;
// typedef double Float;

// typedef int Int;
// typedef long Int;

// vector and matrix types
// these have no geometric meaning
typedef Vec2<int> Vec2i;
typedef Vec2<uint32_t> Vec2u;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;
typedef Vec3<uint32_t> Vec3u;
typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<int> Vec4i;
typedef Vec4<uint32_t> Vec4u;

// TODO: make all of the following use one Float and one Int type
// these represent actual geometric primitives
typedef Point2<float> Point2f;
typedef Point2<double> Point2d;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Point3<float> Point3f;
typedef Point3<double> Point3d;
typedef Normal3<float> Normal3f;
typedef Normal3<double> Normal3d;
typedef Color3<float> Color3f;

typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

typedef imedit::Image<float> Imagef;
typedef imedit::Image<int> Imagei;
typedef imedit::Image<double> Imaged;

typedef BBox3<float> BBox3f;
typedef BBox3<double> BBox3d;

inline float degToRad(float value) { return value * (M_PI / 180.0f); }

#if !defined(_GNU_SOURCE)
    /// Emulate sincosf using sinf() and cosf()
    inline void sincosf(float theta, float *_sin, float *_cos) {
        *_sin = sinf(theta);
        *_cos = cosf(theta);
    }
#endif
