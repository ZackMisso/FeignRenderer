#pragma once

#include <feign/common.h>

template <typename T>
struct BBox3
{
    BBox3()
    {
        min = Vec3<T>(0.0);
        max = Vec3<T>(0.0);
    }

    BBox3(Vec3<T> tlc, T w, T h, T d)
    {
        min = tlc;
        max = tlc + Vec3<T>(w, h, d);
    }

    BBox3(Vec3<T> tlc, Vec3<T> blc)
    {
        min = tlc;
        max = blc;
    }

    Vec3<T> min;
    Vec3<T> max;
};
