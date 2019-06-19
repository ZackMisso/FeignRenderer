#include <feign/misc/intersection.h>

Intersection::Intersection() : intersected_mesh(nullptr) { }

Vector3f Intersection::toLocal(const Vector3f& vec) const
{
    throw new NotImplementedException("intersection to local");

    return Vector3f(0.f);
}

Vector3f Intersection::toWorld(const Vector3f& vec) const
{
    throw new NotImplementedException("intersection to world");

    return Vector3f(0.f);
}
