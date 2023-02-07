/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/closure.h>

FEIGN_BEGIN()

// TODO: i do not think this is really needed at all anymore

PhotonTracingClosure::PhotonTracingClosure()
    : sampler(nullptr),
      its(nullptr),
      ray(nullptr),
      scene(nullptr),
      wi(Vector3f(0.f, 1.f, 0.f)),
      media(nullptr) {}

PhotonTracingClosure::PhotonTracingClosure(Sampler *sampler,
                                           Intersection *its,
                                           Ray3f *ray,
                                           const Scene *scene)
    : sampler(sampler),
      its(its),
      ray(ray),
      scene(scene),
      wi(Vector3f(0.f, 1.f, 0.f)),
      media(nullptr) {}

FEIGN_END()
