/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/math/warp.h>
#include <tgmath.h>

FEIGN_BEGIN()

Point2f WarpSpace::sqrToUniDisk(const Point2f &sample)
{
    Float r = std::sqrt(sample(1));
    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * (sample(0)), &sinPhi, &cosPhi);

    return Point2f(Float(cosPhi) * r, Float(sinPhi) * r);
}

Float WarpSpace::sqrToUniDiskPdf(const Point2f &p)
{
    return p.sqrNorm() <= 1 ? INV_PI : 0.0f;
}

Vector3f WarpSpace::sqrToUniSph(const Point2f &sample)
{
    Float z = 1.0f - 2.0f * sample(1);
    Float r = std::sqrt(std::max((Float) 0.0f, 1.0f - z*z));
    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);
    return Vector3f(r * Float(cosPhi), r * Float(sinPhi), z);
}

Float WarpSpace::sqrToUniSphPdf(const Vector3f &v)
{
    return INV_FOURPI;
}

Vector3f WarpSpace::sqrToUniSphCap(const Point2f &sample, Float cosThetaMax)
{
    Float z = sample(1) + (1.f-sample(1)) * cosThetaMax;
    Float r = std::sqrt(1-z*z);
    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);
    return Vector3f(r * Float(cosPhi), r * Float(sinPhi), z);
}

Float WarpSpace::sqrToUniSphCapPdf(const Vector3f &v, Float cosThetaMax)
{
    if (v(2) < cosThetaMax)
        return 0.0f;
    return INV_TWOPI / (1.0f - cosThetaMax);
}

Vector3f WarpSpace::sqrToUniHemi(const Point2f &sample)
{
    Float cosTheta = 1.0 - sample(1); // TODO: verify this is ok
    Float sinTheta = std::sqrt(std::max((Float) 0, 1-cosTheta*cosTheta));

    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);

    return Vector3f(Float(cosPhi) * sinTheta, Float(sinPhi) * sinTheta, cosTheta);
}

Float WarpSpace::sqrToUniHemiPdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_TWOPI : 0.0f;
}

Vector3f WarpSpace::sqrToCosHemi(const Point2f &sample)
{
    Point2f p = sqrToUniDisk(sample);
    Float z = std::sqrt(std::max((Float) 0, (Float)1.0 - p(0)*p(0) - p(1)*p(1)));

    return Vector3f(p(0), p(1), z);
}

Float WarpSpace::sqrToCosHemiPdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_PI * v(2) : 0.0f;
}

Vector3f WarpSpace::sqrToCosPowHemi(const Point2f &sample, Float n)
{
    Float cosTheta  = std::pow(sample(1), 1.0f / (n + 1.0f));
    Float sinTheta  = std::sqrt(std::max((Float) 0.0f, 1.0f - cosTheta * cosTheta));

    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);

    return Vector3f(Float(cosPhi) * sinTheta, Float(sinPhi) * sinTheta, cosTheta);
}

Float WarpSpace::sqrToCosPowHemiPdf(const Vector3f &v, Float n)
{
    return v(2) > 0 ? 0.5f * (n + 1.0f) * std::pow(v(2), n) * INV_PI : 0.0f;
}

Point2f WarpSpace::sqrToUniTri(const Point2f &sample)
{
    Float u = 1 - sqrt(sample(0));
    Float v = sample(1) * sqrt(sample(0));

    return Point2f(u, v);
}

Float WarpSpace::sqrToUniTriPdf(const Point2f &sample)
{
    if (sample(0) + sample(1) > 1.0) return 0.0f;
    if (sample(0) + sample(1) < 0.0) return 0.0f;

    return 2.0f;
}

Vector3f WarpSpace::sqrToBeck(const Point2f &sample, Float alpha)
{
    throw new NotImplementedException("square to beckmann");

    return Vector3f(0.f);
}

Float WarpSpace::sqrToBeckPdf(const Vector3f &m, Float alpha)
{
    throw new NotImplementedException("square to beckmann pdf");

    return 0.f;
}

FEIGN_END()
