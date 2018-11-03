#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <feign/exceptions.h>
#include <feign/math/vector.h>
#include <feign/math/bbox.h>
#include <feign/log/logger.h>

using namespace std;

// TODO: remove this
#define SCENES_PATH "/Users/corneria/Documents/Projects/FeignRenderer/scenes/"
// #define SCENES_PATH "/Users/venom/Documents/Projects/FeignRenderer/scenes/"

#define Epsilon 1e-4f

#define PI           3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f

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

typedef BBox3<float> BBox3f;
typedef BBox3<double> BBox3d;

typedef float Float;

static Matrix4f Matrix4f_identity()
{
    Matrix4f mat;

    mat[0] = 1.0; mat[1] = 0.0; mat[2] = 0.0; mat[3] = 0.0;
    mat[4] = 0.0; mat[5] = 1.0; mat[6] = 0.0; mat[7] = 0.0;
    mat[8] = 0.0; mat[9] = 0.0; mat[10] = 1.0; mat[11] = 0.0;
    mat[12] = 0.0; mat[13] = 0.0; mat[14] = 0.0; mat[15] = 1.0;

    return mat;
}

static Matrix3f Matrix3f_identity()
{
    Matrix3f mat;

    mat[0] = 1.0; mat[1] = 0.0; mat[2] = 0.0;
    mat[3] = 0.0; mat[4] = 1.0; mat[5] = 0.0;
    mat[6] = 0.0; mat[7] = 0.0; mat[8] = 1.0;

    return mat;
}

inline float degToRad(float value) { return value * (M_PI / 180.0f); }
