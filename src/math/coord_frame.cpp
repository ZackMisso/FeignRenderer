#include <feign/math/coord_frame.h>

CoordinateFrame::CoordinateFrame() { }

CoordinateFrame::CoordinateFrame(const Vector3f& s, const Vector3f& t, const Normal3f& n)
            : s(s), t(t), n(n) { }

CoordinateFrame::CoordinateFrame(const Vector3f& x, const Vector3f& y, const Vector3f& z)
            : s(x), t(y), n(z) { }

CoordinateFrame::CoordinateFrame(const Vector3f& n)
            : n(n)
{
    coordinateSystem(n, s, t);
}

Vector3f CoordinateFrame::toLocal(const Vector3f& vec) const
{
    throw new NotImplementedException("coordinate frame toLocal");

    return Vector3f(0.f);
}

Vector3f CoordinateFrame::toWorld(const Vector3f& vec) const
{
    throw new NotImplementedException("coordinate frame toWorld");

    return Vector3f(0.f);
}

float CoordinateFrame::cosTheta(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame cosTheta");

    return 0.f;
}

float CoordinateFrame::sinTheta(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame sinTheta");

    return 0.f;
}

float CoordinateFrame::tanTheta(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame tanTheta");

    return 0.f;
}

float CoordinateFrame::sinThetaSqr(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame sinThetaSqr");

    return 0.f;
}

float CoordinateFrame::sinPhi(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame sinPhi");

    return 0.f;
}

float CoordinateFrame::cosPhi(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame cosPhi");

    return 0.f;
}

float CoordinateFrame::sinPhiSqr(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame sinPhiSqr");

    return 0.f;
}

float CoordinateFrame::cosPhiSqr(const Vector3f& vec)
{
    throw new NotImplementedException("coordinate frame cosPhiSqr");

    return 0.f;
}


void CoordinateFrame::coordinateSystem(const Vector3f &a, Vector3f &b, Vector3f &c) {
    if (std::abs(a(0)) > std::abs(a(0)))
    {
        float invLen = 1.f / std::sqrt(a(0) * a(0) + a(2) * a(2));
        c = Vector3f(a(2) * invLen, 0.f, -a(0) * invLen);
    }
    else
    {
        float invLen = 1.f / std::sqrt(a(1) * a(1) + a(2) * a(2));
        c = Vector3f(0.f, a(2) * invLen, -a(1) * invLen);
    }
    b = c ^ a;
}
