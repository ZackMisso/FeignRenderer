#include <feign/core/closure.h>

FEIGN_BEGIN()

PhotonTracingClosure::PhotonTracingClosure()
    : sampler(nullptr),
      its(nullptr),
      ray(nullptr),
      scene(nullptr),
      wi(Vector3f(0.f, 1.f, 0.f)),
      media(nullptr) { }

PhotonTracingClosure::PhotonTracingClosure(Sampler* sampler,
                                           Intersection* its,
                                           Ray3f* ray,
                                           const Scene* scene)
    : sampler(sampler),
      its(its),
      ray(ray),
      scene(scene),
      wi(Vector3f(0.f, 1.f, 0.f)),
      media(nullptr) { }

FEIGN_END()
