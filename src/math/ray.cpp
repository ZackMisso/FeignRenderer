#include <feign/math/ray.h>

Ray3f::Ray3f()
{
    origin = Point3f(0.f);
    dir = Vector3f(0.f, 0.f, 1.f);
    mint = Epsilon;
    maxt = 1000000;
}

Ray3f::Ray3f(Vector3f d) : dir(d)
{
    origin = Point3f(0.f);
    mint = Epsilon;
    maxt = 1000000;
}

Ray3f::Ray3f(Point3f o, Vector3f d) : origin(o), dir(d)
{
    mint = Epsilon;
    maxt = 1000000;
}

Ray3f::Ray3f(Point3f o, Vector3f d, Float mint, Float maxt)
    : origin(o), dir(d), mint(mint), maxt(maxt)
{ }

Ray3f::Ray3f(const Ray3f& other)
{
    origin = other.origin;
    dir = other.dir;
    mint = other.mint;
    maxt = other.maxt;
}

Point3f Ray3f::operator()(Float t) const
{
    throw new NotImplementedException("ray3f(t)");
    // TODO
    return Point3f();
}

Ray3f Ray3f::operator~() const
{
    throw new NotImplementedException("ray3f ~ operator");
    // TODO
    return Ray3f();
}
