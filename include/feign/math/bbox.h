#pragma once

#include <feign/common.h>
#include <feign/math/ray.h>

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

    BBox3(Vec3<T> tlc, Vec3<T> blc)
    {
        // The following assert breaks when both min and max have been reset
        // assert(blc[0] >= tlc[0] && blc[1] >= tlc[1] && blc[2] >= tlc[2]);

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

        return 2.0 * (diag(1) * diag(2) +
                      diag(0) * diag(2) +
                      diag(0) * diag(1));
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

    void expand(const Vec3<T>& p)
    {
        min = min.min(p);
        max = max.max(p);
    }

    void expand(const BBox3<T>& bbox)
    {
        // std::cout << ""
        min = min.min(bbox.min);
        max = max.max(bbox.max);
    }

    static BBox3<T> merge(const BBox3<T>& one, const BBox3<T>& two)
    {
        // TODO: rename these methods because they are not intuitive
        Vec3<T> min_vec = one.min.min(two.min);
        Vec3<T> max_vec = one.max.max(two.max);

        // std::cout << "One:" << std::endl;
        // one.infoDump();
        //
        // std::cout << "Two:" << std::endl;
        // two.infoDump();

        // std::cout << "Pre Merge" << std::endl;
        // BBox3<T> box = BBox3<T>(min_vec, max_vec);
        // std::cout << "Post Merge" << std::endl;

        return BBox3<T>(min_vec, max_vec);
    }

    // TODO: ray intersection of bbox might have to happen elsewhere
    bool intersect(const Ray3f& ray) const
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
                if (origin < minVal || origin > maxVal) return false;
            }
            else
            {
                float t1 = (minVal - origin) * (1.0 / ray.dir(i)); // ray.dRcp[i];
                float t2 = (maxVal - origin) * (1.0 / ray.dir(i)); // ray.dRcp[i];

                if (t1 > t2) std::swap(t1, t2);

                nearT = std::max(t1, nearT);
                farT = std::min(t2, farT);

                if (!(nearT <= farT)) return false;
            }
        }

        return ray.near <= farT && nearT <= ray.far;
    }

    bool intersect(const Ray3f& ray, float& near, float& far) const
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
                if (origin < minVal || origin > maxVal) return false;
            }
            else
            {
                float t1 = (minVal - origin) * (1.0 / ray.dir(i)); // ray.dRcp[i];
                float t2 = (maxVal - origin) * (1.0 / ray.dir(i)); // ray.dRcp[i];

                if (t1 > t2) std::swap(t1, t2);

                near = std::max(t1, near);
                far = std::min(t2, far);

                if (!(near <= far)) return false;
            }
        }

        return ray.near <= far && near <= ray.far;
    }

    void reset()
    {
        min = Vec3<T>(std::numeric_limits<T>::infinity());
        max = Vec3<T>(-std::numeric_limits<T>::infinity());
    }

    void infoDump() const
    {
        std::cout << "Min: " << "(" << min(0) << ", " << min(1) << ", " << min(2) << ")" << std::endl;
        std::cout << "Max: " << "(" << max(0) << ", " << max(1) << ", " << max(2) << ")" << std::endl;
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

    BBox2(Vec2<T> tlc, Vec2<T> blc)
    {
        min = tlc;
        max = blc;
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

    bool containsOrOnBoundry(const Vec2<T>& p) const
    {
        return p >= min && p <= max;
    }

    bool contains(const BBox2<T>& bbox) const
    {
        return bbox.min > min && bbox.max < max;
    }

    bool containsOrOnBoundry(const BBox2<T>& bbox) const
    {
        return bbox.min >= min && bbox.max <= max;
    }

    bool overlapExcludeBoundry(const BBox2<T>& bbox) const
    {
        return bbox.min < min && bbox.max > max;
    }

    bool overlaps(const BBox2<T>& bbox) const
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

    void expand(const Vec2<T>& p)
    {
        min = min.min(p);
        max = max.max(p);
    }

    void expand(const BBox2<T>& bbox)
    {
        // std::cout << ""
        min = min.min(bbox.min);
        max = max.max(bbox.max);
    }

    void clip(const Vec2<T>& n_min, const Vec2<T>& n_max)
    {
        if (min(0) < n_min(0)) min[0] = n_min(0);
        if (min(1) < n_min(1)) min[1] = n_min(1);
        if (max(0) > n_max(0)) max[0] = n_max(0);
        if (max(1) > n_max(1)) max[1] = n_max(1);
    }

    static BBox2<T> merge(const BBox2<T>& one, const BBox2<T>& two)
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
        std::cout << "Min: " << "(" << min(0) << ", " << min(1) << ")" << std::endl;
        std::cout << "Max: " << "(" << max(0) << ", " << max(1) << ")" << std::endl;
    }

    Vec2<T> min;
    Vec2<T> max;
};

typedef BBox3<Float> BBox3f;
typedef BBox2<Float> BBox2f;
typedef BBox2<int> BBox2i;
