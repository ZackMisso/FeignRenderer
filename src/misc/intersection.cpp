/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/misc/intersection.h>

Intersection::Intersection()
    : intersected_mesh(nullptr),
      medium(nullptr) { }

Vector3f Intersection::toLocal(const Vector3f& vec) const
{
    return s_frame.toLocal(vec);
}

Vector3f Intersection::toWorld(const Vector3f& vec) const
{
    return s_frame.toWorld(vec);
}

Vector3f Intersection::toLocalGeom(const Vector3f& vec) const
{
    return g_frame.toLocal(vec);
}

Vector3f Intersection::toWorldGeom(const Vector3f& vec) const
{
    return g_frame.toWorld(vec);
}
