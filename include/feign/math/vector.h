#pragma once

// I do not really think any complex templates are needed for now
// the added virtual tables would only slow down the current renders.
// these are kept as separate data structures for the sole purpose
// of improving code readability and debugging

#include <feign/common.h>
#include <nanogui/glutil.h>

template <typename T>
struct Vec2
{
    Vec2(T c) { }
    Vec2(T x, T y) { }

};

template <typename T>
struct Vec3
{
    Vec3(T c)
    {
        xyz[0] = c;
        xyz[1] = c;
        xyz[2] = c;
    }

    Vec3(T x, T y, T z)
    {
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
    }

    Vec3(Vec2<T> xy, T z)
    {
        xyz[0] = xy[0];
        xyz[1] = xy[1];
        xyz[z] = z;
    }

    Vec3<T> operator+(const Vec3<T>& other) const
    {
        Vec3<T> vec;
        vec[0] = xyz[0] + other[0];
        vec[1] = xyz[1] + other[1];
        vec[2] = xyz[2] + other[2];
        return vec;
    }

    Vec3<T> operator-(const Vec3<T>& other) const
    {
        Vec3<T> vec;
        vec[0] = xyz[0] - other[0];
        vec[1] = xyz[1] - other[1];
        vec[2] = xyz[2] - other[2];
        return vec;
    }

    Vec3<T> operator*(T c) const
    {
        Vec3<T> vec;
        vec[0] = xyz[0] * c;
        vec[1] = xyz[1] * c;
        vec[2] = xyz[2] * c;
        return vec;
    }

    Vec3<T> operator/(T c) const
    {
        Vec3<T> vec;
        vec[0] = xyz[0] / c;
        vec[1] = xyz[1] / c;
        vec[2] = xyz[2] / c;
        return vec;
    }

    void operator+=(const Vec3<T>& other)
    {
        xyz[0] += other[0];
        xyz[1] += other[1];
        xyz[2] += other[2];
    }

    void operator-=(const Vec3<T>& other)
    {
        xyz[0] -= other[0];
        xyz[1] -= other[1];
        xyz[2] -= other[2];
    }

    void operator*=(const Vec3<T>& other)
    {
        xyz[0] *= other[0];
        xyz[1] *= other[1];
        xyz[2] *= other[2];
    }

    void operator/=(const Vec3<T>& other)
    {
        xyz[0] /= other[0];
        xyz[1] /= other[1];
        xyz[2] /= other[2];
    }

    Vec3<T> operator~()
    {
        Vec3<T> vec;
        vec[0] = -xyz[0];
        vec[1] = -xyz[1];
        vec[2] = -xyz[2];
        return vec;
    }

    T xyz[3];
};

// fuck eigen. Im going to write my own as a challenge

// // gross shit
// template <typename Scalar, int Dimension>
// struct ZVector : public Eigen::Matrix<Scalar, Dimension, 1> {
// public:
//
//     typedef Eigen::Matrix<Scalar, Dimension, 1> Parent;
//     typedef ZVector<Scalar, Dimension>          VectorType;
//     typedef ZVector<Scalar, Dimension>          PointType;
//
//     ZVector()
//     {
//         Parent::setConstant(0);
//     }
//
//     ZVector(Scalar x)
//     {
//         Parent::setConstant(x);
//     }
//
//     ZVector(Scalar x, Scalar y) : Parent(x, y)
//     {
//         // nothing
//     }
//
//     ZVector(Scalar x, Scalar y, Scalar z) : Parent(x, y, z)
//     {
//         // nothing
//     }
//
//     ZVector(Scalar x, Scalar y, Scalar z, Scalar w) : Parent(x, y, z, w)
//     {
//         // nothing
//     }
// };

template <typename T>
struct Vector2
{
    Vector2();
    Vector2(T c);
    Vector2(T x, T y);

    T x;
    T y;
};

template <typename T>
struct Vector3
{
    Vector3();
    Vector3(T c);
    Vector3(T x, T y);
    Vector3(T x, T y, T z);
    Vector3(Vector2<T> xy);
    Vector3(Vector2<T> xy, T z);

    T x;
    T y;
    T z;
};

template <typename T>
struct Vector4
{
    Vector4();
    Vector4(T c);
    Vector4(T x, T y);
    Vector4(T x, T y, T z);
    Vector4(T x, T y, T z, T w);
    Vector4(Vector2<T> xy);
    Vector4(Vector2<T> xy, Vector2<T> zw);
    Vector4(Vector3<T> xyz, T w);

    T x;
    T y;
    T z;
    T w;
};

struct Matrix3
{
    // TODO
};

struct Matrix4
{
    // TODO
};

struct Vector3f
{
    Vector3f();
    Vector3f(Float c);
    Vector3f(Float x, Float y, Float z);

    Float x;
    Float y;
    Float z;
};

struct Point3f
{
    Point3f();
    Point3f(Float c);
    Point3f(Float x, Float y, Float z);

    Float x;
    Float y;
    Float z;
};

struct Normal3f
{
    Normal3f();
    Normal3f(Float c);
    Normal3f(Float x, Float y, Float z);

    Float x;
    Float y;
    Float z;
};
