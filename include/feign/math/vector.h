#pragma once

// I do not really think any complex templates are needed for now
// the added virtual tables would only slow down the current renders.
// these are kept as separate data structures for the sole purpose
// of improving code readability and debugging

#include <feign/common.h>
#include <feign/exceptions.h>
// #include <nanogui/glutil.h>

template <typename T>
struct Vec2
{
    Vec2(T c)
    {
        xy[0] = c;
        xy[1] = c;
    }

    Vec2(T x, T y)
    {
        xy[0] = x;
        xy[1] = y;
    }

    Vec2<T> operator+(const Vec2<T>& other) const
    {
        Vec2<T> vec;
        vec[0] = xy[0] + other(0);
        vec[1] = xy[1] + other(1);
        return vec;
    }

    Vec2<T> operator-(const Vec2<T>& other) const
    {
        Vec2<T> vec;
        vec[0] = xy[0] - other(0);
        vec[1] = xy[1] - other(1);
        return vec;
    }

    Vec2<T> operator*(T c) const
    {
        Vec2<T> vec;
        vec[0] = xy[0] * c;
        vec[1] = xy[1] * c;
        return vec;
    }

    Vec2<T> operator/(T c) const
    {
        Vec2<T> vec;
        vec[0] = xy[0] / c;
        vec[1] = xy[1] / c;
        return vec;
    }

    void operator+=(const Vec2<T>& other)
    {
        xy[0] += other(0);
        xy[1] += other(1);
    }

    void operator-=(const Vec2<T>& other)
    {
        xy[0] -= other(0);
        xy[1] -= other(1);
    }

    // component wise multiplication
    void operator*=(const Vec2<T>& other)
    {
        xy[0] *= other(0);
        xy[1] *= other(1);
    }

    void operator/=(const Vec2<T>& other)
    {
        xy[0] /= other(0);
        xy[1] /= other(1);
    }

    // dot product
    T operator%(Vec2<T>& other)
    {
        return other(0) * xy[0] +
               other(1) * xy[1];
    }

    // cross product
    T operator^(const Vec2<T>& other)
    {
        return xy[0] * other(1) - xy[1] * other(0);
    }

    // returns a reference
    T& operator[](int index)
    {
        return xy[index];
    }

    // returns a copy
    T operator()(int index) const
    {
        return xy[index];
    }

    Vec2<T> operator~()
    {
        Vec2<T> vec;
        vec[0] = -xy[0];
        vec[1] = -xy[1];
        return vec;
    }

    T xy[2];
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
        xyz[0] = xy(0);
        xyz[1] = xy(1);
        xyz[2] = z;
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

    // component wise multiplication
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

    // dot product
    T operator%(Vec3<T>& other)
    {
        return other[0] * xyz[0] +
               other[1] * xyz[1] +
               other[2] * xyz[2];
    }

    // cross product
    Vec3<T> operator^(const Vec3<T>& other)
    {
        Vec3<T> vec;
        vec[0] = xyz[2] * other[3] - xyz[3] * other[2];
        vec[1] = xyz[3] * other[1] - xyz[1] * other[3];
        vec[2] = xyz[1] * other[2] - xyz[2] * other[1];
        return vec;
    }

    // returns a reference
    T& operator[](int index)
    {
        return xyz[index];
    }

    // returns a copy
    T operator()(int index) const
    {
        return xyz[index];
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

template <typename T>
struct Vec4
{
    Vec4(T c)
    {
        xyzw[0] = c;
        xyzw[1] = c;
        xyzw[2] = c;
        xyzw[3] = c;
    }

    Vec4(T x, T y, T z, T w)
    {
        xyzw[0] = x;
        xyzw[1] = y;
        xyzw[2] = z;
        xyzw[3] = w;
    }

    Vec4(Vec2<T> xy, T z, T w)
    {
        xyzw[0] = xy(0);
        xyzw[1] = xy(1);
        xyzw[2] = z;
        xyzw[3] = w;
    }

    Vec4(Vec2<T> xy, Vec2<T> zw)
    {
        xyzw[0] = xy(0);
        xyzw[1] = xy(1);
        xyzw[2] = zw(0);
        xyzw[3] = zw(1);
    }

    Vec4<T> operator+(const Vec4<T>& other) const
    {
        Vec4<T> vec;
        vec[0] = xyzw[0] + other(0);
        vec[1] = xyzw[1] + other(1);
        vec[2] = xyzw[2] + other(2);
        vec[3] = xyzw[3] + other(3);
        return vec;
    }

    Vec4<T> operator-(const Vec4<T>& other) const
    {
        Vec4<T> vec;
        vec[0] = xyzw[0] - other(0);
        vec[1] = xyzw[1] - other(1);
        vec[2] = xyzw[2] - other(2);
        vec[3] = xyzw[3] - other(3);
        return vec;
    }

    Vec4<T> operator*(T c) const
    {
        Vec4<T> vec;
        vec[0] = xyzw[0] * c;
        vec[1] = xyzw[1] * c;
        vec[2] = xyzw[2] * c;
        vec[3] = xyzw[3] * c;
        return vec;
    }

    Vec3<T> operator/(T c) const
    {
        Vec3<T> vec;
        vec[0] = xyzw[0] / c;
        vec[1] = xyzw[1] / c;
        vec[2] = xyzw[2] / c;
        return vec;
    }

    void operator+=(const Vec4<T>& other)
    {
        xyzw[0] += other(0);
        xyzw[1] += other(1);
        xyzw[2] += other(2);
        xyzw[3] += other(3);
    }

    void operator-=(const Vec4<T>& other)
    {
        xyzw[0] -= other(0);
        xyzw[1] -= other(1);
        xyzw[2] -= other(2);
        xyzw[3] -= other(3);
    }

    // component wise multiplication
    void operator*=(const Vec4<T>& other)
    {
        xyzw[0] *= other(0);
        xyzw[1] *= other(1);
        xyzw[2] *= other(2);
        xyzw[3] *= other(3);
    }

    void operator/=(const Vec4<T>& other)
    {
        xyzw[0] /= other(0);
        xyzw[1] /= other(1);
        xyzw[2] /= other(2);
        xyzw[3] /= other(3);
    }

    // dot product
    T operator%(Vec4<T>& other)
    {
        return other(0) * xyzw[0] +
               other(1) * xyzw[1] +
               other(2) * xyzw[2] +
               other(3) * xyzw[3];
    }

    // cross product - not supported/needed for Vec4s
    // Vec4<T> operator^(const Vec4<T>& other)
    // {
    //     Vec3<T> vec;
    //     vec[0] = xyzw[2] * other[3] - xyzw[3] * other[2];
    //     vec[1] = xyzw[3] * other[1] - xyzw[1] * other[3];
    //     vec[2] = xyzw[1] * other[2] - xyzw[2] * other[1];
    //     return vec;
    // }

    // returns a reference
    T& operator[](int index)
    {
        return xyzw[index];
    }

    // returns a copy
    T operator()(int index) const
    {
        return xyzw[index];
    }

    Vec4<T> operator~()
    {
        Vec4<T> vec;
        vec[0] = -xyzw[0];
        vec[1] = -xyzw[1];
        vec[2] = -xyzw[2];
        vec[3] = -xyzw[3];
        return vec;
    }

    T xyzw[4];
};

template <typename T>
struct Matrix3
{
    Matrix3()
    {
        for (int i = 0; i < 9; ++i)
        {
            n[i] = 0x0;
        }
    }

    Matrix3(T n00, T n01, T n02,
            T n10, T n11, T n12,
            T n20, T n21, T n22)
    {
        n[0] = n00; n[1] = n01; n[2] = n02;
        n[3] = n10; n[4] = n11; n[5] = n12;
        n[6] = n20; n[7] = n21; n[8] = n22;
    }

    Matrix3<T> operator*(const Matrix3<T>& other)
    {
        Matrix3<T> mat;

        mat[0] = n[0]*other(0) + n[1]*other(3) + n[2]*other(6);
        mat[1] = n[0]*other(1) + n[1]*other(4) + n[2]*other(7);
        mat[2] = n[0]*other(2) + n[1]*other(5) + n[2]*other(8);

        mat[3] = n[3]*other(0) + n[4]*other(3) + n[5]*other(6);
        mat[4] = n[3]*other(1) + n[4]*other(4) + n[5]*other(7);
        mat[5] = n[3]*other(2) + n[4]*other(5) + n[5]*other(8);

        mat[6] = n[6]*other(0) + n[7]*other(3) + n[8]*other(6);
        mat[7] = n[6]*other(1) + n[7]*other(4) + n[8]*other(7);
        mat[8] = n[6]*other(2) + n[7]*other(5) + n[8]*other(8);

        return mat;
    }

    Matrix3<T> operator+(const Matrix3<T>& other)
    {
        Matrix3<T> mat;

        for (int i = 0; i < 9; ++i)
            mat[i] = n[i] + other(i);

        return mat;
    }

    Matrix3<T> operator-(const Matrix3<T>& other)
    {
        Matrix3<T> mat;

        for (int i = 0; i < 9; ++i)
            mat[i] = n[i] - other(i);

        return mat;
    }

    // transpose
    Matrix3<T> operator!()
    {
        Matrix3<T> mat;

        throw new NotImplementedException();

        return mat;
    }

    // inverse
    Matrix3<T> operator~()
    {
        Matrix3<T> mat;

        throw new NotImplementedException();

        return mat;
    }

    // returns a reference
    T& operator[](int index)
    {
        return n[index];
    }

    // returns a copy
    T operator()(int index) const
    {
        return n[index];
    }

    T n[9];
};

template <typename T>
struct Matrix4
{
    Matrix4()
    {
        for (int i = 0; i < 16; ++i)
        {
            n[i] = 0x0;
        }
    }

    Matrix4(T n00, T n01, T n02, T n03,
            T n10, T n11, T n12, T n13,
            T n20, T n21, T n22, T n23,
            T n30, T n31, T n32, T n33)
    {
        n[0] = n00; n[1] = n01; n[2] = n02; n[3] = n03;
        n[4] = n10; n[5] = n11; n[6] = n12; n[7] = n13;
        n[8] = n20; n[9] = n21; n[10] = n22; n[11] = n23;
        n[12] = n30; n[13] = n31; n[14] = n32; n[15] = n33;
    }

    Matrix4<T> operator*(const Matrix4<T>& other)
    {
        Matrix4<T> mat;

        mat[0] = n[0]*other(0) + n[1]*other(4) + n[2]*other(8) + n[3]*other(12);
        mat[1] = n[0]*other(1) + n[1]*other(5) + n[2]*other(9) + n[3]*other(13);
        mat[2] = n[0]*other(2) + n[1]*other(6) + n[2]*other(10) + n[3]*other(14);
        mat[3] = n[0]*other(3) + n[1]*other(7) + n[2]*other(11) + n[3]*other(15);

        mat[4] = n[4]*other(0) + n[5]*other(4) + n[6]*other(8) + n[7]*other(12);
        mat[5] = n[4]*other(1) + n[5]*other(5) + n[6]*other(9) + n[7]*other(13);
        mat[6] = n[4]*other(2) + n[5]*other(6) + n[6]*other(10) + n[7]*other(14);
        mat[7] = n[4]*other(3) + n[5]*other(7) + n[6]*other(11) + n[7]*other(15);

        mat[8] = n[8]*other(0) + n[9]*other(4) + n[10]*other(8) + n[11]*other(12);
        mat[9] = n[8]*other(1) + n[9]*other(5) + n[10]*other(9) + n[11]*other(13);
        mat[10] = n[8]*other(2) + n[9]*other(6) + n[10]*other(10) + n[11]*other(14);
        mat[11] = n[8]*other(3) + n[9]*other(7) + n[10]*other(11) + n[11]*other(15);

        mat[12] = n[12]*other(0) + n[13]*other(4) + n[14]*other(8) + n[15]*other(12);
        mat[13] = n[12]*other(1) + n[13]*other(5) + n[14]*other(9) + n[15]*other(13);
        mat[14] = n[12]*other(2) + n[13]*other(6) + n[14]*other(10) + n[15]*other(14);
        mat[15] = n[12]*other(3) + n[13]*other(7) + n[14]*other(11) + n[15]*other(15);

        return mat;
    }

    Matrix4<T> operator+(const Matrix4<T>& other)
    {
        Matrix4<T> mat;

        for (int i = 0; i < 16; ++i)
            mat[i] = n[i] + other(i);

        return mat;
    }

    Matrix4<T> operator-(const Matrix4<T>& other)
    {
        Matrix4<T> mat;

        for (int i = 0; i < 16; ++i)
            mat[i] = n[i] - other(i);

        return mat;
    }

    // transpose
    Matrix4<T> operator!()
    {
        Matrix4<T> mat;

        throw new NotImplementedException();

        return mat;
    }

    // inverse
    Matrix4<T> operator~()
    {
        Matrix4<T> mat;

        throw new NotImplementedException();

        return mat;
    }

    // returns a reference
    T& operator[](int index)
    {
        return n[index];
    }

    // returns a copy
    T operator()(int index) const
    {
        return n[index];
    }

    T n[16];
};

template <typename T>
struct Vector3 : Vec3<T>
{
    Vector3(T c) : Vec3(c) { }
    Vector3(T x, T y, T z) : Vec3(x, y, z) { }
    Vector3(Vec2<T> xy, T z) : Vec3(xy, z) { }
};

template <typename T>
struct Point3 : Vec3<T>
{
    Point3(T c) : Vec3(c) { }
    Point3(T x, T y, T z) : Vec3(x, y, z) { }
    Point3(Vec2<T> xy, T z) : Vec3(xy, z) { }
};

template <typename T>
struct Normal3 : Vec3<T>
{
    Normal3(T c) : Vec3(c) { }
    Normal3(T x, T y, T z) : Vec3(x, y, z) { }
    Normal3(Vec2<T> xy, T z) : Vec3(xy, z) { }
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

// template <typename T>
// struct Vector2
// {
//     Vector2();
//     Vector2(T c);
//     Vector2(T x, T y);
//
//     T x;
//     T y;
// };
//
// template <typename T>
// struct Vector3
// {
//     Vector3();
//     Vector3(T c);
//     Vector3(T x, T y);
//     Vector3(T x, T y, T z);
//     Vector3(Vector2<T> xy);
//     Vector3(Vector2<T> xy, T z);
//
//     T x;
//     T y;
//     T z;
// };
//
// template <typename T>
// struct Vector4
// {
//     Vector4();
//     Vector4(T c);
//     Vector4(T x, T y);
//     Vector4(T x, T y, T z);
//     Vector4(T x, T y, T z, T w);
//     Vector4(Vector2<T> xy);
//     Vector4(Vector2<T> xy, Vector2<T> zw);
//     Vector4(Vector3<T> xyz, T w);
//
//     T x;
//     T y;
//     T z;
//     T w;
// };

// struct Matrix3
// {
//     // TODO
// };
//
// struct Matrix4
// {
//     // TODO
// };

// struct Vector3f
// {
//     Vector3f();
//     Vector3f(Float c);
//     Vector3f(Float x, Float y, Float z);
//
//     Float x;
//     Float y;
//     Float z;
// };
//
// struct Point3f
// {
//     Point3f();
//     Point3f(Float c);
//     Point3f(Float x, Float y, Float z);
//
//     Float x;
//     Float y;
//     Float z;
// };
//
// struct Normal3f
// {
//     Normal3f();
//     Normal3f(Float c);
//     Normal3f(Float x, Float y, Float z);
//
//     Float x;
//     Float y;
//     Float z;
// };
