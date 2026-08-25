/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/misc/intersection.h>

FEIGN_BEGIN()

Intersection::Intersection()
    : intersected_mesh(nullptr) {}

Intersection::Intersection(const Intersection &its)
    : p(its.p),
      uv(its.uv),
      t(its.t),
      f(its.f),
      bary(its.bary),
      s_frame(its.s_frame),
      g_frame(its.g_frame),
      intersected_mesh(its.intersected_mesh) {}

Vector3f Intersection::to_local(const Vector3f &vec) const
{
    return s_frame.to_local(vec);
}

Vector3f Intersection::to_world(const Vector3f &vec) const
{
    return s_frame.to_world(vec);
}

Vector3f Intersection::to_local_geom(const Vector3f &vec) const
{
    return g_frame.to_local(vec);
}

Vector3f Intersection::to_world_geom(const Vector3f &vec) const
{
    return g_frame.to_world(vec);
}

FEIGN_END()
