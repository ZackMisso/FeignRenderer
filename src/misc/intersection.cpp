#include <feign/misc/intersection.h>

Intersection::Intersection() : intersected_mesh(nullptr) { }

Vector3f Intersection::toLocal(const Vector3f& vec) const
{
    return s_frame.toLocal(vec);
}

Vector3f Intersection::toWorld(const Vector3f& vec) const
{
    return s_frame.toWorld(vec);
}
