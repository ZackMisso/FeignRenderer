/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/math/coord_frame.h>

FEIGN_BEGIN()

CoordinateFrame::CoordinateFrame() {}

CoordinateFrame::CoordinateFrame(const CoordinateFrame &frame)
    : s(frame.s),
      t(frame.t),
      n(frame.n) {}

CoordinateFrame::CoordinateFrame(const Vector3f &s,
                                 const Vector3f &t,
                                 const Normal3f &n)
    : s(s), t(t), n(n) {}

CoordinateFrame::CoordinateFrame(const Vector3f &x,
                                 const Vector3f &y,
                                 const Vector3f &z)
    : s(x), t(y), n(z) {}

CoordinateFrame::CoordinateFrame(const Normal3f &n)
    : n(n)
{
    coordinateSystem(n, s, t);
}

Vector3f CoordinateFrame::toLocal(const Vector3f &vec) const
{
    return Vector3f((vec % s),
                    (vec % t),
                    (vec % n));
}

Vector3f CoordinateFrame::toWorld(const Vector3f &vec) const
{
    return s * vec(0) +
           t * vec(1) +
           n * vec(2);
}

// assumes the input is in local space
Float CoordinateFrame::cosTheta(const Vector3f &vec)
{
    return vec(2);
}

Float CoordinateFrame::sinTheta(const Vector3f &vec)
{
    throw new NotImplementedException("coordinate frame sinTheta");

    return 0.f;
}

Float CoordinateFrame::tanTheta(const Vector3f &vec)
{
    throw new NotImplementedException("coordinate frame tanTheta");

    return 0.f;
}

Float CoordinateFrame::sinThetaSqr(const Vector3f &vec)
{
    throw new NotImplementedException("coordinate frame sinThetaSqr");

    return 0.f;
}

Float CoordinateFrame::sinPhi(const Vector3f &vec)
{
    throw new NotImplementedException("coordinate frame sinPhi");

    return 0.f;
}

Float CoordinateFrame::cosPhi(const Vector3f &vec)
{
    throw new NotImplementedException("coordinate frame cosPhi");

    return 0.f;
}

Float CoordinateFrame::sinPhiSqr(const Vector3f &vec)
{
    throw new NotImplementedException("coordinate frame sinPhiSqr");

    return 0.f;
}

Float CoordinateFrame::cosPhiSqr(const Vector3f &vec)
{
    throw new NotImplementedException("coordinate frame cosPhiSqr");

    return 0.f;
}

void CoordinateFrame::coordinateSystem(const Vector3f &a, Vector3f &b, Vector3f &c)
{
    if (std::abs(a(0)) > std::abs(a(1)))
    {
        Float invLen = 1.f / std::sqrt(a(0) * a(0) + a(2) * a(2));
        c = Vector3f(a(2) * invLen, 0.f, -a(0) * invLen);
    }
    else
    {
        Float invLen = 1.f / std::sqrt(a(1) * a(1) + a(2) * a(2));
        c = Vector3f(0.f, a(2) * invLen, -a(1) * invLen);
    }
    b = c ^ a;
}

Vector3f CoordinateFrame::sphericalDirection(Float cos_theta,
                                             Float sin_theta,
                                             Float phi,
                                             const Vector3f &x,
                                             const Vector3f &y,
                                             const Vector3f &z)
{
    return x * std::cos(phi) * sin_theta +
           y * std::sin(phi) * sin_theta +
           z * cos_theta;
}

FEIGN_END()
