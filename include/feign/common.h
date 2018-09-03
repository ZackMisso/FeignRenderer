#pragma once

#include <string>
#include <vector>
#include <iostream>

// #include <nanogui/glutil.h>

#include <feign/exceptions.h>
#include <feign/math/vector.h>

using namespace std;

#define Epsilon 1e-4f

#define PI           3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f

// vector and matrix types
typedef Vec2<int> Vector2i;
typedef Vec2<uint32_t> Vector2u;
typedef Vec2<float> Vector2f;
typedef Vec2<double> Vector2d;
typedef Vec4<float> Vector4f;
typedef Vec4<double> Vector4d;

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Point3<float> Point3f;
typedef Point3<double> Point3d;
typedef Normal3<float> Normal3f;
typedef Normal3<double> Normal3d;

typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

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
