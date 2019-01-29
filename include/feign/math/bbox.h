#pragma once

#include <feign/common.h>
// #include <feign/math/ray.h>

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

    T volume() const
    {
        return (max.x - min.x) *
               (max.y - min.y) *
               (max.z - min.z);
    }

    T surfaceArea() const
    {
        Vec3<T> diag = max - min;

        return 2.0 * (diag[1] * diag[2] +
                      diag[0] * diag[2] +
                      diag[0] * diag[1]);
    }

    Vec3<T> center() const
    {
        return (max + min) * (T)0.5;
    }

    bool contains(const Vec3<T>& p) const
    {
        return p > min && p < max;
    }

    bool containsOrOnBoundry(const Vec3<T>& p) const
    {
        return p >= min && p <= max;
    }

    bool contains(const BBox3<T>& bbox) const
    {
        return bbox.min > min && bbox.max < max;
    }

    bool containsOrOnBoundry(const BBox3<T>& bbox) const
    {
        return bbox.min >= min && bbox.max <= max;
    }

    bool overlapExcludeBoundry(const BBox3<T>& bbox) const
    {
        return bbox.min < min && bbox.max > max;
    }

    bool overlaps(const BBox3<T>& bbox) const
    {
        return bbox.min <= min && bbox.max >= max;
    }

    int majorAxis() const
    {
        Vec3<T> diag = max - min;
        return diag.maxIndex();
    }

    int minorAxis() const
    {
        Vec3<T> diag = max - min;
        return diag.minIndex();
    }

    Vec3<T> extents() const
    {
        return max - min;
    }

    void expand(const Vec3<T>& p)
    {
        min = min.min(p);
        max = max.max(p);
    }

    void expand(const BBox3<T>& bbox)
    {
        min = min.min(bbox.min);
        max = max.max(bbox.max);
    }

    static BBox3<T> merge(const BBox3<T>& one, const BBox3<T>& two)
    {
        return BBox3<T>(one.min(two), one.max(two));
    }

    // TODO: ray intersection of bbox might have to happen elsewhere
    // bool intersect(const Ray3f& ray) const
    // {
    //     // TODO
    //     return false;
    // }
    //
    // bool intersect(const Ray3f& ray, Float& near, Float& far) const
    // {
    //     // TODO
    //     return false;
    // }

    Vec3<T> min;
    Vec3<T> max;
};
