/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

// I do not really think any complex templates are needed for now
// the added virtual tables would only slow down the current renders.
// these are kept as separate data structures for the sole purpose
// of improving code readability and debugging

#include <feign/common.h>
#include <tgmath.h>

template <typename T>
struct Vec2
{
    Vec2()
    {
        xy[0] = 0x0;
        xy[1] = 0x0;
    }

    Vec2(const Vec2<T>& other)
    {
        xy[0] = other(0);
        xy[1] = other(1);
    }

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

    Vec2<T> operator-() const
    {
        Vec2<T> vec;
        vec[0] = -xy[0];
        vec[1] = -xy[1];
        return vec;
    }

    bool operator<(const Vec2<T>& other) const
    {
        return (xy[0] < other.xy(0) &&
                xy[1] < other.xy(1));
    }

    bool operator<(T c) const
    {
        return (xy[0] < c &&
                xy[1] < c);
    }

    bool operator>(const Vec2<T>& other) const
    {
        return (xy[0] > other.xy[0] &&
                xy[1] > other.xy[1]);
    }

    bool operator>(T c) const
    {
        return (xy[0] > c &&
                xy[1] > c);
    }

    bool operator<=(const Vec2<T>& other) const
    {
        return (xy[0] <= other.xy[0] &&
                xy[1] <= other.xy[1]);
    }

    bool operator<=(T c) const
    {
        return (xy[0] <= c &&
                xy[1] <= c);
    }

    bool operator>=(const Vec2<T>& other) const
    {
        return (xy[0] >= other.xy[0] &&
                xy[1] >= other.xy[1]);
    }

    bool operator>=(T c) const
    {
        return (xy[0] >= c &&
                xy[1] >= c);
    }

    bool operator==(const Vec2<T>& other) const
    {
        return (xy[0] == other.xy[0] &&
                xy[1] == other.xy[1]);
    }

    bool operator==(T c) const
    {
        return (xy[0] == c &&
                xy[1] == c);
    }

    bool operator!=(const Vec2<T>& other) const
    {
        return (xy[0] != other.xy[0] &&
                xy[1] != other.xy[1]);
    }

    bool operator!=(T c) const
    {
        return (xy[0] != c &&
                xy[1] != c);
    }

    // dot product
    T operator%(const Vec2<T>& other) const
    {
        return other(0) * xy[0] +
               other(1) * xy[1];
    }

    // cross product
    T operator^(const Vec2<T>& other) const
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

    T sqrNorm() const
    {
        return xy[0] * xy[0] + xy[1] * xy[1];
    }

    T norm() const
    {
        return sqrt(xy[0] * xy[0] + xy[1] * xy[1]);
    }

    Vec2<T> normalized() const
    {
        T n = norm();
        Vec2<T> vec;
        vec[0] = xy[0] / n;
        vec[1] = xy[1] / n;
        return vec;
    }

    Vec2<T> operator~() const
    {
        Vec2<T> vec;

        if (xy[0] < 0.0) vec[0] = -xy[0];
        else vec[0] = xy[0];

        if (xy[1] < 0.0) vec[1] = -xy[1];
        else vec[1] = xy[1];

        return vec;
    }

    Vec2<T> min(const Vec2<T>& other) const
    {
        Vec2<T> vec;

        if (xy[0] < other.xy[0]) vec.xy[0] = xy[0];
        else vec.xy[0] = other.xy[0];

        if (xy[1] < other.xy[1]) vec.xy[1] = xy[1];
        else vec.xy[1] = other.xy[1];

        return vec;
    }

    Vec2<T> max(const Vec2<T>& other) const
    {
        Vec2<T> vec;

        if (xy[0] > other.xy[0]) vec.xy[0] = xy[0];
        else vec.xy[0] = other.xy[0];

        if (xy[1] > other.xy[1]) vec.xy[1] = xy[1];
        else vec.xy[1] = other.xy[1];

        return vec;
    }

    int maxIndex() const
    {
        return xy[0] > xy[1] ? 0 : 1;
    }

    int minIndex() const
    {
        return xy[0] < xy[1] ? 0 : 1;
    }

    int maxAbsIndex() const
    {
        return std::abs(xy[0]) > std::abs(xy[1]) ? 0 : 1;
    }

    int minAbsIndex() const
    {
        return std::abs(xy[0]) < std::abs(xy[1]) ? 0 : 1;
    }

    void info() const
    {
        std::cout << "(" << xy[0] << ", " << xy[1] << ")" << std::endl;
    }

    T xy[2];
};

template <typename T>
struct Vec3
{
    Vec3()
    {
        xyz[0] = 0x0;
        xyz[1] = 0x0;
        xyz[2] = 0x0;
    }

    Vec3(const Vec3<T>& other)
    {
        xyz[0] = other(0);
        xyz[1] = other(1);
        xyz[2] = other(2);
    }

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
        xyz[2] = z;
    }

    Vec3<T> operator+(const Vec3<T>& other) const
    {
        Vec3<T> vec;
        vec[0] = xyz[0] + other(0);
        vec[1] = xyz[1] + other(1);
        vec[2] = xyz[2] + other(2);
        return vec;
    }

    Vec3<T> operator-(const Vec3<T>& other) const
    {
        Vec3<T> vec;
        vec[0] = xyz[0] - other(0);
        vec[1] = xyz[1] - other(1);
        vec[2] = xyz[2] - other(2);
        return vec;
    }

    Vec3<T> operator*(const Vec3<T>& other) const
    {
        Vec3<T> vec;
        vec[0] = xyz[0] * other(0);
        vec[1] = xyz[1] * other(1);
        vec[2] = xyz[2] * other(2);
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
        xyz[0] += other(0);
        xyz[1] += other(1);
        xyz[2] += other(2);
    }

    void operator-=(const Vec3<T>& other)
    {
        xyz[0] -= other(0);
        xyz[1] -= other(1);
        xyz[2] -= other(2);
    }

    // component wise multiplication
    void operator*=(const Vec3<T>& other)
    {
        xyz[0] *= other(0);
        xyz[1] *= other(1);
        xyz[2] *= other(2);
    }

    void operator/=(const Vec3<T>& other)
    {
        xyz[0] /= other(0);
        xyz[1] /= other(1);
        xyz[2] /= other(2);
    }

    // dot product
    T operator%(const Vec3<T>& other) const
    {
        return other(0) * xyz[0] +
               other(1) * xyz[1] +
               other(2) * xyz[2];
    }

    // cross product
    Vec3<T> operator^(const Vec3<T>& other) const
    {
        Vec3<T> vec;
        vec[0] = xyz[1] * other(2) - xyz[2] * other(1);
        vec[1] = xyz[2] * other(0) - xyz[0] * other(2);
        vec[2] = xyz[0] * other(1) - xyz[1] * other(0);
        return vec;
    }

    Vec3<T> operator-() const
    {
        Vec3<T> vec;
        vec[0] = -xyz[0];
        vec[1] = -xyz[1];
        vec[2] = -xyz[2];
        return vec;
    }

    bool operator<(const Vec3<T>& other) const
    {
        return (xyz[0] < other.xyz[0] &&
                xyz[1] < other.xyz[1] &&
                xyz[2] < other.xyz[2]);
    }

    bool operator<(T c) const
    {
        return (xyz[0] < c &&
                xyz[1] < c &&
                xyz[2] < c);
    }

    bool operator>(const Vec3<T>& other) const
    {
        return (xyz[0] > other.xyz[0] &&
                xyz[1] > other.xyz[1] &&
                xyz[2] > other.xyz[2]);
    }

    bool operator>(T c) const
    {
        return (xyz[0] > c &&
                xyz[1] > c &&
                xyz[2] > c);
    }

    bool operator<=(const Vec3<T>& other) const
    {
        return (xyz[0] <= other.xyz[0] &&
                xyz[1] <= other.xyz[1] &&
                xyz[2] <= other.xyz[2]);
    }

    bool operator<=(T c) const
    {
        return (xyz[0] <= c &&
                xyz[1] <= c &&
                xyz[2] <= c);
    }

    bool operator>=(const Vec3<T>& other) const
    {
        return (xyz[0] >= other.xyz[0] &&
                xyz[1] >= other.xyz[1] &&
                xyz[2] >= other.xyz[2]);
    }

    bool operator>=(T c) const
    {
        return (xyz[0] >= c &&
                xyz[1] >= c &&
                xyz[2] >= c);
    }

    bool operator==(const Vec3<T>& other) const
    {
        return (xyz[0] == other.xyz[0] &&
                xyz[1] == other.xyz[1] &&
                xyz[2] == other.xyz[2]);
    }

    bool operator==(T c) const
    {
        return (xyz[0] == c &&
                xyz[1] == c &&
                xyz[2] == c);
    }

    bool operator!=(const Vec3<T>& other) const
    {
        return (xyz[0] != other.xyz[0] &&
                xyz[1] != other.xyz[1] &&
                xyz[2] != other.xyz[2]);
    }

    bool operator!=(T c) const
    {
        return (xyz[0] != c &&
                xyz[1] != c &&
                xyz[2] != c);
    }

    T sqrNorm() const
    {
        return xyz[0] * xyz[0] +
               xyz[1] * xyz[1] +
               xyz[2] * xyz[2];
    }

    T norm() const
    {
        return sqrt(xyz[0] * xyz[0] +
                    xyz[1] * xyz[1] +
                    xyz[2] * xyz[2]);
    }

    Vec3<T> normalized() const
    {
        T n = norm();
        Vec3<T> vec;
        vec[0] = xyz[0] / n;
        vec[1] = xyz[1] / n;
        vec[2] = xyz[2] / n;
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

    Vec3<T> operator~() const
    {
        Vec3<T> vec;

        if (xyz[0] < 0.0) vec[0] = -xyz[0];
        else vec[0] = xyz[0];

        if (xyz[1] < 0.0) vec[1] = -xyz[1];
        else vec[1] = xyz[1];

        if (xyz[2] < 0.0) vec[2] = -xyz[2];
        else vec[2] = xyz[2];

        return vec;
    }

    Vec3<T> min(const Vec3<T>& other) const
    {
        Vec3<T> vec;

        if (xyz[0] < other.xyz[0]) vec.xyz[0] = xyz[0];
        else vec.xyz[0] = other.xyz[0];

        if (xyz[1] < other.xyz[1]) vec.xyz[1] = xyz[1];
        else vec.xyz[1] = other.xyz[1];

        if (xyz[2] < other.xyz[2]) vec.xyz[2] = xyz[2];
        else vec.xyz[2] = other.xyz[2];

        return vec;
    }

    Vec3<T> max(const Vec3<T>& other) const
    {
        Vec3<T> vec;

        if (xyz[0] > other.xyz[0]) vec.xyz[0] = xyz[0];
        else vec.xyz[0] = other.xyz[0];

        if (xyz[1] > other.xyz[1]) vec.xyz[1] = xyz[1];
        else vec.xyz[1] = other.xyz[1];

        if (xyz[2] > other.xyz[2]) vec.xyz[2] = xyz[2];
        else vec.xyz[2] = other.xyz[2];

        return vec;
    }

    int maxIndex() const
    {
        if (xyz[0] > xyz[1] &&
            xyz[0] > xyz[2]) return 0;
        if (xyz[1] > xyz[0] &&
            xyz[1] > xyz[2]) return 1;
        return 2;
    }

    int minIndex() const
    {
        if (xyz[0] < xyz[1] &&
            xyz[0] < xyz[2]) return 0;
        if (xyz[1] < xyz[0] &&
            xyz[1] < xyz[2]) return 1;
        return 2;
    }

    int maxAbsIndex() const
    {
        if (std::abs(xyz[0]) > std::abs(xyz[1]) &&
            std::abs(xyz[0]) > std::abs(xyz[2])) return 0;
        if (std::abs(xyz[1]) > std::abs(xyz[0]) &&
            std::abs(xyz[1]) > std::abs(xyz[2])) return 1;
        return 2;
    }

    int minAbsIndex() const
    {
        if (std::abs(xyz[0]) < std::abs(xyz[1]) &&
            std::abs(xyz[0]) < std::abs(xyz[2])) return 0;
        if (std::abs(xyz[1]) < std::abs(xyz[0]) &&
            std::abs(xyz[1]) < std::abs(xyz[2])) return 1;
        return 2;
    }

    void info() const
    {
        std::cout << "(" << xyz[0] << ", " <<
                            xyz[1] << ", " <<
                            xyz[2] << ")" << std::endl;
    }

    T xyz[3];
};

template <typename T>
struct Vec4
{
    Vec4()
    {
        xyzw[0] = 0x0;
        xyzw[1] = 0x0;
        xyzw[2] = 0x0;
        xyzw[3] = 0x0;
    }

    Vec4(const Vec4<T>& other)
    {
        xyzw[0] = other(0);
        xyzw[1] = other(1);
        xyzw[2] = other(2);
        xyzw[3] = other(3);
    }

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

    Vec4(Vec3<T> xyz, T w)
    {
        xyzw[0] = xyz(0);
        xyzw[1] = xyz(1);
        xyzw[2] = xyz(2);
        xyzw[3] = w;
    }

    Vec3<T> head() const
    {
        return Vec3<T>(xyzw[0], xyzw[1], xyzw[2]);
    }

    Vec4<T> operator+(const Vec4<T>& other) const
    {
        Vec4<T> vec;
        vec[0] = xyzw(0) + other(0);
        vec[1] = xyzw(1) + other(1);
        vec[2] = xyzw(2) + other(2);
        vec[3] = xyzw(3) + other(3);
        return vec;
    }

    Vec4<T> operator-(const Vec4<T>& other) const
    {
        Vec4<T> vec;
        vec[0] = xyzw(0) - other(0);
        vec[1] = xyzw(1) - other(1);
        vec[2] = xyzw(2) - other(2);
        vec[3] = xyzw(3) - other(3);
        return vec;
    }

    Vec4<T> operator*(T c) const
    {
        Vec4<T> vec;
        vec[0] = xyzw(0) * c;
        vec[1] = xyzw(1) * c;
        vec[2] = xyzw(2) * c;
        vec[3] = xyzw(3) * c;
        return vec;
    }

    Vec3<T> operator/(T c) const
    {
        Vec3<T> vec;
        vec[0] = xyzw(0) / c;
        vec[1] = xyzw(1) / c;
        vec[2] = xyzw(2) / c;
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
    T operator%(const Vec4<T>& other) const
    {
        return other(0) * xyzw(0) +
               other(1) * xyzw(1) +
               other(2) * xyzw(2) +
               other(3) * xyzw(3);
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

    T sqrNorm() const
    {
        return xyzw(0) * xyzw(0) +
               xyzw(1) * xyzw(1) +
               xyzw(2) * xyzw(2) +
               xyzw(3) * xyzw(3);
    }

    T norm() const
    {
        return sqrt(xyzw(0) * xyzw(0) +
                    xyzw(1) * xyzw(1) +
                    xyzw(2) * xyzw(2) +
                    xyzw(3) * xyzw(3));
    }

    Vec4<T> normalized() const
    {
        T n = norm();
        Vec4<T> vec;
        vec[0] = xyzw(0) / n;
        vec[1] = xyzw(1) / n;
        vec[2] = xyzw(2) / n;
        vec[3] = xyzw(3) / n;
        return vec;
    }

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

        if (xyzw[0] < 0.0) vec[0] = -xyzw[0];
        else vec[0] = xyzw[0];

        if (xyzw[1] < 0.0) vec[1] = -xyzw[1];
        else vec[1] = xyzw[1];

        if (xyzw[2] < 0.0) vec[2] = -xyzw[2];
        else vec[2] = xyzw[2];

        if (xyzw[3] < 0.0) vec[3] = -xyzw[3];
        else vec[3] = xyzw[3];

        return vec;
    }

    bool operator<(const Vec4<T>& other)
    {
        return (xyzw[0] < other.xyzw[0] &&
                xyzw[1] < other.xyzw[1] &&
                xyzw[2] < other.xyzw[2] &&
                xyzw[3] < other.xyzw[3]);
    }

    bool operator<(T c)
    {
        return (xyzw[0] < c &&
                xyzw[1] < c &&
                xyzw[2] < c &&
                xyzw[3] < c);
    }

    bool operator>(const Vec4<T>& other)
    {
        return (xyzw[0] > other.xyzw[0] &&
                xyzw[1] > other.xyzw[1] &&
                xyzw[2] > other.xyzw[2] &&
                xyzw[3] > other.xyzw[3]);
    }

    bool operator>(T c)
    {
        return (xyzw[0] > c &&
                xyzw[1] > c &&
                xyzw[2] > c &&
                xyzw[3] > c);
    }

    bool operator<=(const Vec4<T>& other)
    {
        return (xyzw[0] <= other.xyzw[0] &&
                xyzw[1] <= other.xyzw[1] &&
                xyzw[2] <= other.xyzw[2] &&
                xyzw[3] <= other.xyzw[3]);
    }

    bool operator<=(T c)
    {
        return (xyzw[0] <= c &&
                xyzw[1] <= c &&
                xyzw[2] <= c &&
                xyzw[3] <= c);
    }

    bool operator>=(const Vec4<T>& other)
    {
        return (xyzw[0] >= other.xyzw[0] &&
                xyzw[1] >= other.xyzw[1] &&
                xyzw[2] >= other.xyzw[2] &&
                xyzw[3] >= other.xyzw[3]);
    }

    bool operator>=(T c)
    {
        return (xyzw[0] >= c &&
                xyzw[1] >= c &&
                xyzw[2] >= c &&
                xyzw[3] >= c);
    }

    Vec4<T> min(const Vec4<T>& other) const
    {
        Vec4<T> vec;

        if (xyzw[0] < other.xyzw[0]) vec.xyzw[0] = xyzw[0];
        else vec.xyzw[0] = other.xyzw[0];

        if (xyzw[1] < other.xyzw[1]) vec.xyzw[1] = xyzw[1];
        else vec.xyzw[1] = other.xyzw[1];

        if (xyzw[2] < other.xyzw[2]) vec.xyzw[2] = xyzw[2];
        else vec.xyzw[2] = other.xyzw[2];

        if (xyzw[3] < other.xyzw[3]) vec.xyzw[3] = xyzw[3];
        else vec.xyzw[3] = other.xyzw[3];

        return vec;
    }

    Vec4<T> max(const Vec4<T>& other) const
    {
        Vec4<T> vec;

        if (xyzw[0] > other.xyzw[0]) vec.xyzw[0] = xyzw[0];
        else vec.xyzw[0] = other.xyzw[0];

        if (xyzw[1] > other.xyzw[1]) vec.xyzw[1] = xyzw[1];
        else vec.xyzw[1] = other.xyzw[1];

        if (xyzw[2] > other.xyzw[2]) vec.xyzw[2] = xyzw[2];
        else vec.xyzw[2] = other.xyzw[2];

        if (xyzw[3] > other.xyzw[3]) vec.xyzw[3] = xyzw[3];
        else vec.xyzw[3] = other.xyzw[3];

        return vec;
    }

    int maxIndex() const
    {
        int max = 0;
        T max_val = xyzw[0];

        for (int i = 1; i < 4; ++i)
        {
            if (xyzw[i] > max_val)
            {
                max_val = xyzw[i];
                max = i;
            }
        }

        return max;
    }

    int minIndex() const
    {
        int min = 0;
        T min_val = xyzw[0];

        for (int i = 1; i < 4; ++i)
        {
            if (xyzw[i] < min_val)
            {
                min_val = xyzw[i];
                min = i;
            }
        }

        return min;
    }

    int maxAbsIndex() const
    {
        int max = 0;
        T max_val = std::abs(xyzw[0]);

        for (int i = 1; i < 4; ++i)
        {
            if (std::abs(xyzw[i]) > max_val)
            {
                max_val = std::abs(xyzw[i]);
                max = i;
            }
        }

        return max;
    }

    int minAbsIndex() const
    {
        int min = 0;
        T min_val = std::abs(xyzw[0]);

        for (int i = 1; i < 4; ++i)
        {
            if (std::abs(xyzw[i]) < min_val)
            {
                min_val = std::abs(xyzw[i]);
                min = i;
            }
        }

        return min;
    }

    void info() const
    {
        std::cout << "(" << xyzw[0] << ", " <<
                            xyzw[1] << ", " <<
                            xyzw[2] << ", " <<
                            xyzw[3] << ")" << std::endl;
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

    static Matrix3<T> identity()
    {
        Matrix3<T> mat;

        mat[0] = (T)1.0; mat[1] = (T)0.0; mat[2] = (T)0.0;
        mat[3] = (T)0.0; mat[4] = (T)1.0; mat[5] = (T)0.0;
        mat[6] = (T)0.0; mat[7] = (T)0.0; mat[8] = (T)1.0;

        return mat;
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

    Vec3<T> operator*(const Vec3<T> other)
    {
        Vec3<T> vec;

        vec[0] = n[0]*other(0) + n[1]*other(1) + n[2]*other(2);
        vec[1] = n[3]*other(0) + n[4]*other(1) + n[5]*other(2);
        vec[2] = n[6]*other(0) + n[7]*other(1) + n[8]*other(2);

        return vec;
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
    Matrix3<T> operator!() const
    {
        Matrix3<T> mat;

        mat.n[0] = n[0];
        mat.n[1] = n[3];
        mat.n[2] = n[6];
        mat.n[3] = n[1];
        mat.n[4] = n[4];
        mat.n[5] = n[7];
        mat.n[6] = n[2];
        mat.n[7] = n[5];
        mat.n[8] = n[8];

        return mat;
    }

    // inverse
    Matrix3<T> operator~() const
    {
        Matrix3<T> mat;

        assert(false);

        // throw new NotImplementedException("Matrix3 inverse");

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

    Matrix3<T> topLeftCorner() const
    {
        Matrix3<T> mat;

        mat[0] = n[0]; mat[1] = n[1]; mat[2] = n[2];
        mat[3] = n[4]; mat[4] = n[5]; mat[5] = n[6];
        mat[6] = n[8]; mat[7] = n[9]; mat[8] = n[10];

        return mat;
    }

    static Matrix4<T> identity()
    {
        Matrix4<T> mat;

        mat[0] = (T)1.0; mat[1] = (T)0.0; mat[2] = (T)0.0; mat[3] = (T)0.0;
        mat[4] = (T)0.0; mat[5] = (T)1.0; mat[6] = (T)0.0; mat[7] = (T)0.0;
        mat[8] = (T)0.0; mat[9] = (T)0.0; mat[10] = (T)1.0; mat[11] = (T)0.0;
        mat[12] = (T)0.0; mat[13] = (T)0.0; mat[14] = (T)0.0; mat[15] = (T)1.0;

        return mat;
    }

    static Matrix4<T> scale(const Vec3<T>& vec)
    {
        Matrix4<T> mat = Matrix4<T>();

        mat[0] = vec(0);
        mat[5] = vec(1);
        mat[10] = vec(2);
        mat[15] = 1.0;

        return mat;
    }

    static Matrix4<T> translate(const Vec3<T>& vec)
    {
        Matrix4<T> mat = Matrix4<T>();

        mat[0] = 1.0;
        mat[5] = 1.0;
        mat[10] = 1.0;
        mat[15] = 1.0;
        mat[3] = vec(0);
        mat[7] = vec(1);
        mat[11] = vec(2);

        return mat;
    }

    Matrix4<T> operator*(const Matrix4<T>& other) const
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

    Vec4<T> operator*(const Vec4<T>& other) const
    {
        Vec4<T> vec;

        vec[0] = n[0]*other(0) + n[1]*other(1) + n[2]*other(2) + n[3]*other(3);
        vec[1] = n[4]*other(0) + n[5]*other(1) + n[6]*other(2) + n[7]*other(3);
        vec[2] = n[8]*other(0) + n[9]*other(1) + n[10]*other(2) + n[11]*other(3);
        vec[3] = n[12]*other(0) + n[13]*other(1) + n[14]*other(2) + n[15]*other(3);

        return vec;
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
    Matrix4<T> operator!() const
    {
        Matrix4<T> mat;

        mat[0] = n[0]; mat[1] = n[4]; mat[2] = n[8]; mat[3] = n[12];
        mat[4] = n[1]; mat[5] = n[5]; mat[6] = n[9]; mat[7] = n[13];
        mat[8] = n[2]; mat[9] = n[6]; mat[10] = n[10]; mat[11] = n[14];
        mat[12] = n[3]; mat[13] = n[7]; mat[14] = n[11]; mat[15] = n[15];

        return mat;
    }

    // inverse
    Matrix4<T> operator~() const
    {
        Matrix4<T> mat;

        T det = 0;

        det += n[0] * (n[5] * n[10] * n[15] + n[6] * n[11] * n[13] +
                       n[7] * n[9] * n[14] - n[7] * n[10] * n[13] -
                       n[6] * n[9] * n[15] - n[5] * n[11] * n[14]);

        det -= n[4] * (n[1] * n[10] * n[15] + n[2] * n[11] * n[13] +
                       n[3] * n[9] * n[14] - n[3] * n[10] * n[13] -
                       n[2] * n[9] * n[15] - n[1] * n[11] * n[14]);

        det += n[8] * (n[1] * n[6] * n[15] + n[2] * n[7] * n[13] +
                       n[3] * n[5] * n[14] - n[3] * n[6] * n[13] -
                       n[2] * n[5] * n[15] - n[1] * n[7] * n[14]);

        det -= n[12] * (n[1] * n[6] * n[11] + n[2] * n[7] * n[9] +
                        n[3] * n[5] * n[10] - n[3] * n[6] * n[9] -
                        n[2] * n[5] * n[11] - n[1] * n[7] * n[10]);

        T m11 = n[5] * n[10] * n[15] + n[6] * n[11] * n[13] +
                n[7] * n[9] * n[14] - n[7] * n[10] * n[13] -
                n[6] * n[9] * n[15] - n[5] * n[11] * n[14];

        T m12 = n[4] * n[10] * n[15] + n[6] * n[11] * n[12] +
                n[7] * n[8] * n[14] - n[7] * n[10] * n[12] -
                n[6] * n[8] * n[15] - n[4] * n[11] * n[14];

        T m13 = n[4] * n[9] * n[15] + n[5] * n[11] * n[12] +
                n[7] * n[8] * n[13] - n[7] * n[9] * n[12] -
                n[5] * n[8] * n[15] - n[4] * n[11] * n[13];

        T m14 = n[4] * n[9] * n[14] + n[5] * n[10] * n[12] +
                n[6] * n[8] * n[13] - n[6] * n[9] * n[12] -
                n[5] * n[8] * n[14] - n[4] * n[10] * n[13];

        T m21 = n[1] * n[10] * n[15] + n[2] * n[11] * n[13] +
                n[3] * n[9] * n[14] - n[3] * n[10] * n[13] -
                n[2] * n[9] * n[15] - n[1] * n[11] * n[14];

        T m22 = n[0] * n[10] * n[15] + n[2] * n[11] * n[12] +
                n[3] * n[8] * n[14] - n[3] * n[10] * n[12] -
                n[2] * n[8] * n[15] - n[0] * n[11] * n[14];

        T m23 = n[0] * n[9] * n[15] + n[1] * n[11] * n[12] +
                n[3] * n[8] * n[13] - n[3] * n[9] * n[12] -
                n[1] * n[8] * n[15] - n[0] * n[11] * n[13];

        T m24 = n[0] * n[9] * n[14] + n[1] * n[10] * n[12] +
                n[2] * n[8] * n[13] - n[2] * n[9] * n[12] -
                n[1] * n[8] * n[14] - n[0] * n[10] * n[13];

        T m31 = n[1] * n[6] * n[15] + n[2] * n[7] * n[13] +
                n[3] * n[5] * n[14] - n[3] * n[6] * n[13] -
                n[2] * n[5] * n[15] - n[1] * n[7] * n[14];

        T m32 = n[0] * n[6] * n[15] + n[2] * n[7] * n[12] +
                n[3] * n[4] * n[14] - n[3] * n[6] * n[12] -
                n[2] * n[4] * n[15] - n[0] * n[7] * n[14];

        T m33 = n[0] * n[5] * n[15] + n[1] * n[7] * n[12] +
                n[3] * n[4] * n[13] - n[3] * n[5] * n[12] -
                n[1] * n[4] * n[15] - n[0] * n[7] * n[13];

        T m34 = n[0] * n[5] * n[14] + n[1] * n[6] * n[12] +
                n[2] * n[4] * n[13] - n[2] * n[5] * n[12] -
                n[1] * n[4] * n[14] - n[0] * n[6] * n[13];

        T m41 = n[1] * n[6] * n[11] + n[2] * n[7] * n[9] +
                n[3] * n[5] * n[10] - n[3] * n[6] * n[9] -
                n[2] * n[5] * n[11] - n[1] * n[7] * n[10];

        T m42 = n[0] * n[6] * n[11] + n[2] * n[7] * n[8] +
                n[3] * n[4] * n[10] - n[3] * n[6] * n[8] -
                n[2] * n[4] * n[11] - n[0] * n[7] * n[10];

        T m43 = n[0] * n[5] * n[11] + n[1] * n[7] * n[8] +
                n[3] * n[4] * n[9] - n[3] * n[5] * n[8] -
                n[1] * n[4] * n[11] - n[0] * n[7] * n[9];

        T m44 = n[0] * n[5] * n[10] + n[1] * n[6] * n[8] +
                n[2] * n[4] * n[9] - n[2] * n[5] * n[8] -
                n[1] * n[4] * n[10] - n[0] * n[6] * n[9];

        mat.n[0] = m11 / det;
        mat.n[1] = -m21 / det;
        mat.n[2] = m31 / det;
        mat.n[3] = -m41 / det;
        mat.n[4] = -m12 / det;
        mat.n[5] = m22 / det;
        mat.n[6] = -m32 / det;
        mat.n[7] = m42 / det;
        mat.n[8] = m13 / det;
        mat.n[9] = -m23 / det;
        mat.n[10] = m33 / det;
        mat.n[11] = -m43 / det;
        mat.n[12] = -m14 / det;
        mat.n[13] = m24 / det;
        mat.n[14] = -m34 / det;
        mat.n[15] = m44 / det;

        return mat;
    }

    void setCol(int index, const Vec4<T>& vals)
    {
        operator()(index, 0) = vals(0);
        operator()(index, 1) = vals(1);
        operator()(index, 2) = vals(2);
        operator()(index, 3) = vals(3);
    }

    void setRow(int index, const Vec4<T>& vals)
    {
        operator()(0, index) = vals(0);
        operator()(1, index) = vals(1);
        operator()(2, index) = vals(2);
        operator()(3, index) = vals(3);
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

    T& operator()(int x, int y)
    {
        return n[4 * y + x];
    }

    bool operator==(const Matrix4<T> other) const
    {
        for (int i = 0; i < 16; ++i)
        {
            if (n[i] != other.n[i])
            {
                return false;
            }
        }
        return true;
    }

    T n[16];
};

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
        return Vec3<T>::operator()(0) == 0.0 &&
               Vec3<T>::operator()(1) == 0.0 &&
               Vec3<T>::operator()(2) == 0.0;
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
