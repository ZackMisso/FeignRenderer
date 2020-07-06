/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/misc/intersection.h>

FEIGN_BEGIN()

Intersection::Intersection()
    : intersected_mesh(nullptr) { }

Intersection::Intersection(const Intersection& its)
    : p(its.p),
      uv(its.uv),
      t(its.t),
      f(its.f),
      bary(its.bary),
      s_frame(its.s_frame),
      g_frame(its.g_frame),
      intersected_mesh(its.intersected_mesh) { }

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

FEIGN_END()
