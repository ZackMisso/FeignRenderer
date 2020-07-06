/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/math/ray.h>

FEIGN_BEGIN()

Ray3f::Ray3f()
{
    origin = Point3f(0.f);
    dir = Vector3f(0.f, 0.f, 1.f);
    near = Epsilon;
    far = std::numeric_limits<Float>::infinity();
    depth = 0;
}

Ray3f::Ray3f(Vector3f d) : dir(d)
{
    origin = Point3f(0.f);
    near = Epsilon;
    far = std::numeric_limits<Float>::infinity();
    depth = 0;
}

Ray3f::Ray3f(Point3f o, Vector3f d) : origin(o), dir(d)
{
    near = Epsilon;
    far = std::numeric_limits<Float>::infinity();
    depth = 0;
}

Ray3f::Ray3f(Point3f o, Vector3f d, Float n, Float f, uint32_t depth)
    : origin(o), dir(d), near(n), far(f), depth(depth)
{
}

Ray3f::Ray3f(const Ray3f& other)
{
    origin = other.origin;
    dir = other.dir;
    near = other.near;
    far = other.far;
    depth = other.depth;
}

Point3f Ray3f::operator()(Float t) const
{
    return origin + dir * t;
}

Ray3f Ray3f::operator~() const
{
    throw new NotImplementedException("ray3f ~ operator");
    // TODO
    return Ray3f();
}

FEIGN_END()
