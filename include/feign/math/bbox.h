/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/ray.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

template <typename T>
struct BBox3
{
    BBox3()
    {
        reset();
    }

    BBox3(Vec3<T> vec)
    {
        min = vec;
        max = vec;
    }

    BBox3(Vec3<T> tlc, T w, T h, T d)
    {
        assert(w >= 0 && h >= 0 && d >= 0);

        min = tlc;
        max = tlc + Vec3<T>(w, h, d);
    }

    BBox3(Vec3<T> tlc, Vec3<T> brc)
    {
        min = tlc;
        max = brc;
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

        return 2.0 * (diag(1) * diag(2) +
                      diag(0) * diag(2) +
                      diag(0) * diag(1));
    }

    Vec3<T> center() const
    {
        return (max + min) * (T)0.5;
    }

    bool contains(const Vec3<T> &p) const
    {
        return p > min && p < max;
    }

    bool containsOrOnBoundry(const Vec3<T> &p) const
    {
        return p >= min && p <= max;
    }

    bool contains(const BBox3<T> &bbox) const
    {
        return bbox.min > min && bbox.max < max;
    }

    bool containsOrOnBoundry(const BBox3<T> &bbox) const
    {
        return bbox.min >= min && bbox.max <= max;
    }

    bool overlapExcludeBoundry(const BBox3<T> &bbox) const
    {
        return bbox.min < min && bbox.max > max;
    }

    bool overlaps(const BBox3<T> &bbox) const
    {
        return bbox.min <= min && bbox.max >= max;
    }

    int majorAxis() const
    {
        Vec3<T> diag = max - min;
        return diag.maxAbsIndex();
    }

    int minorAxis() const
    {
        Vec3<T> diag = max - min;
        return diag.minAbsIndex();
    }

    Vec3<T> extents() const
    {
        return max - min;
    }

    void expand(const Vec3<T> &p)
    {
        min = min.min(p);
        max = max.max(p);
    }

    void expand(const BBox3<T> &bbox)
    {
        // std::cout << ""
        min = min.min(bbox.min);
        max = max.max(bbox.max);
    }

    static BBox3<T> merge(const BBox3<T> &one, const BBox3<T> &two)
    {
        // TODO: rename these methods because they are not intuitive
        Vec3<T> min_vec = one.min.min(two.min);
        Vec3<T> max_vec = one.max.max(two.max);

        return BBox3<T>(min_vec, max_vec);
    }

    bool intersect(const Ray3f &ray) const
    {
        float nearT = -std::numeric_limits<float>::infinity();
        float farT = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; i++)
        {
            float origin = ray.origin(i);
            float minVal = min(i);
            float maxVal = max(i);

            if (ray.dir(i) == 0)
            {
                if (origin < minVal || origin > maxVal)
                    return false;
            }
            else
            {
                float t1 = (minVal - origin) * (1.0 / ray.dir(i)); // ray.dRcp[i];
                float t2 = (maxVal - origin) * (1.0 / ray.dir(i)); // ray.dRcp[i];

                if (t1 > t2)
                    std::swap(t1, t2);

                nearT = std::max(t1, nearT);
                farT = std::min(t2, farT);

                if (!(nearT <= farT))
                    return false;
            }
        }

        return ray.near <= farT && nearT <= ray.far;
    }

    // i hope this is correct
    bool intersect(const Ray3f &ray, Float &near, Float &far) const
    {
        near = -std::numeric_limits<float>::infinity();
        far = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; i++)
        {
            float origin = ray.origin(i);
            float minVal = min(i);
            float maxVal = max(i);

            if (ray.dir(i) == 0)
            {
                if (origin < minVal || origin > maxVal)
                    return false;
            }
            else
            {
                Float t1 = (minVal - origin) * (1.0 / ray.dir(i));
                Float t2 = (maxVal - origin) * (1.0 / ray.dir(i));

                if (t1 > t2)
                    std::swap(t1, t2);

                near = std::max(t1, near);
                far = std::min(t2, far);

                if (!(near <= far))
                    return false;
            }
        }

        return ray.near <= far && near <= ray.far;
    }

    void split_into_eight(std::vector<BBox3<T> > &sections) const
    {
        // this returns 8 bounding boxes which are the results of directly splitting
        // the bbox about the midpoints of each of its dimensions.
        // the returned ordering is as follows:
        //
        // min_x - half_x, min_y - half_y, min_z - half_z
        // min_x - half_x, half_y - max_y, min_z - half_z
        // min_x - half_x, min_y - half_y, half_z - max_z
        // min_x - half_x, half_y - max_y, half_z - max_z
        // half_x - max_x, min_y - half_y, min_z - half_z
        // half_x - max_x, half_y - max_y, min_z - half_z
        // half_x - max_x, min_y - half_y, half_z - max_z
        // half_x - max_x, half_y - max_y, half_z - max_z

        Vec3<T> half = extents() / 2.f + min;

        sections.push_back(BBox3<T>(Vec3<T>(min(0), min(1), min(2)),
                                    Vec3<T>(half(0), half(1), half(2))));
        sections.push_back(BBox3<T>(Vec3<T>(min(0), half(1), min(2)),
                                    Vec3<T>(half(0), max(1), half(2))));
        sections.push_back(BBox3<T>(Vec3<T>(min(0), min(1), half(2)),
                                    Vec3<T>(half(0), half(1), max(2))));
        sections.push_back(BBox3<T>(Vec3<T>(min(0), half(1), half(2)),
                                    Vec3<T>(half(0), max(1), max(2))));

        sections.push_back(BBox3<T>(Vec3<T>(half(0), min(1), min(2)),
                                    Vec3<T>(max(0), half(1), half(2))));
        sections.push_back(BBox3<T>(Vec3<T>(half(0), half(1), min(2)),
                                    Vec3<T>(max(0), max(1), half(2))));
        sections.push_back(BBox3<T>(Vec3<T>(half(0), min(1), half(2)),
                                    Vec3<T>(max(0), half(1), max(2))));
        sections.push_back(BBox3<T>(Vec3<T>(half(0), half(1), half(2)),
                                    Vec3<T>(max(0), max(1), max(2))));
    }

    Vec3f operator()(Vec3f sample) const
    {
        return (max - min) * sample;
    }

    Vec3f operator()(Float x, Float y, Float z) const
    {
        return Vec3f((max(0) - min(0)) * x,
                     (max(1) - min(1)) * y,
                     (max(2) - min(2)) * z);
    }

    Point3f local_space(Point3f &point)
    {
        return Point3f((point(0) - min(0)) / (max(0) - min(0)),
                       (point(1) - min(1)) / (max(1) - min(1)),
                       (point(2) - min(2)) / (max(2) - min(2)));
    }

    Float sample_x(Float x) const
    {
        return (max(0) - min(0)) * x;
    }

    Float sample_y(Float y) const
    {
        return (max(0) - min(0)) * y;
    }

    Float sample_z(Float z) const
    {
        return (max(0) - min(0)) * z;
    }

    void reset()
    {
        min = Vec3<T>(std::numeric_limits<T>::infinity());
        max = Vec3<T>(-std::numeric_limits<T>::infinity());
    }

    void infoDump() const
    {
        std::cout << "Min: "
                  << "(" << min(0) << ", " << min(1) << ", " << min(2) << ")" << std::endl;
        std::cout << "Max: "
                  << "(" << max(0) << ", " << max(1) << ", " << max(2) << ")" << std::endl;
    }

    Vec3<T> min;
    Vec3<T> max;
};

template <typename T>
struct BBox2
{
    BBox2()
    {
        reset();
    }

    BBox2(Vec2<T> vec)
    {
        min = vec;
        max = vec;
    }

    BBox2(Vec2<T> tlc, T w, T h)
    {
        assert(w >= 0 && h >= 0);

        min = tlc;
        max = tlc + Vec2<T>(w, h);
    }

    BBox2(Vec2<T> tlc, Vec2<T> brc)
    {
        min = tlc;
        max = brc;
    }

    T area() const
    {
        return (max.x - min.x) * (max.y - min.y);
    }

    Vec2<T> center() const
    {
        (max + min) * T(0.5);
    }

    bool contains(const Vec2<T> p) const
    {
        return p > min && p < max;
    }

    bool containsOrOnBoundry(const Vec2<T> &p) const
    {
        return p >= min && p <= max;
    }

    bool contains(const BBox2<T> &bbox) const
    {
        return bbox.min > min && bbox.max < max;
    }

    bool containsOrOnBoundry(const BBox2<T> &bbox) const
    {
        return bbox.min >= min && bbox.max <= max;
    }

    bool overlapExcludeBoundry(const BBox2<T> &bbox) const
    {
        return bbox.min < min && bbox.max > max;
    }

    bool overlaps(const BBox2<T> &bbox) const
    {
        return bbox.min <= min && bbox.max >= max;
    }

    int majorAxis() const
    {
        Vec2<T> diag = max - min;
        return diag.maxAbsIndex();
    }

    int minorAxis() const
    {
        Vec2<T> diag = max - min;
        return diag.minAbsIndex();
    }

    Vec2<T> extents() const
    {
        return max - min;
    }

    void expand(const Vec2<T> &p)
    {
        min = min.min(p);
        max = max.max(p);
    }

    void expand(const BBox2<T> &bbox)
    {
        // std::cout << ""
        min = min.min(bbox.min);
        max = max.max(bbox.max);
    }

    void clip(const Vec2<T> &n_min, const Vec2<T> &n_max)
    {
        if (min(0) < n_min(0))
            min[0] = n_min(0);
        if (min(1) < n_min(1))
            min[1] = n_min(1);
        if (max(0) > n_max(0))
            max[0] = n_max(0);
        if (max(1) > n_max(1))
            max[1] = n_max(1);
    }

    static BBox2<T> merge(const BBox2<T> &one, const BBox2<T> &two)
    {
        // TODO: rename these methods because they are not intuitive
        Vec2<T> min_vec = one.min.min(two.min);
        Vec2<T> max_vec = one.max.max(two.max);

        return BBox2<T>(min_vec, max_vec);
    }

    void reset()
    {
        min = Vec2<T>(std::numeric_limits<T>::infinity());
        max = Vec2<T>(-std::numeric_limits<T>::infinity());
    }

    void infoDump() const
    {
        std::cout << "Min: "
                  << "(" << min(0) << ", " << min(1) << ")" << std::endl;
        std::cout << "Max: "
                  << "(" << max(0) << ", " << max(1) << ")" << std::endl;
        std::cout << "Extents: " << (max - min).norm() << std::endl;
    }

    Vec2<T> min;
    Vec2<T> max;
};

typedef BBox3<Float> BBox3f;
typedef BBox2<Float> BBox2f;
typedef BBox2<int> BBox2i;

FEIGN_END()
