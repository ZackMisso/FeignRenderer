/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/math/warp.h>
#include <tgmath.h>

Vector3f WarpSpace::sampleUniformHemisphere(Sampler *sampler, const Normal3f& pole)
{
    // Naive implementation using rejection sampling
    Vector3f v;
    do {
        v[0] = 1.f - 2.f * sampler->next1D();
        v[1] = 1.f - 2.f * sampler->next1D();
        v[2] = 1.f - 2.f * sampler->next1D();
    } while (v.sqrNorm() > 1.f);

    if (v % pole < 0.f)
        v = -v;
    v /= v.norm();

    return v;
}

Point2f WarpSpace::squareToUniformDisk(const Point2f &sample)
{
    Float r = std::sqrt(sample(1));
    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * (sample(0)), &sinPhi, &cosPhi);

    return Point2f(Float(cosPhi) * r, Float(sinPhi) * r);
}

Float WarpSpace::squareToUniformDiskPdf(const Point2f &p)
{
    return p.sqrNorm() <= 1 ? INV_PI : 0.0f;
}

Vector3f WarpSpace::squareToUniformSphere(const Point2f &sample)
{
    Float z = 1.0f - 2.0f * sample(1);
    Float r = std::sqrt(std::max((Float) 0.0f, 1.0f - z*z));
    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);
    return Vector3f(r * Float(cosPhi), r * Float(sinPhi), z);
}

Float WarpSpace::squareToUniformSpherePdf(const Vector3f &v)
{
    return INV_FOURPI;
}

Vector3f WarpSpace::squareToUniformSphereCap(const Point2f &sample, Float cosThetaMax)
{
    Float z = sample(1) + (1.f-sample(1)) * cosThetaMax;
    Float r = std::sqrt(1-z*z);
    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);
    return Vector3f(r * Float(cosPhi), r * Float(sinPhi), z);
}

Float WarpSpace::squareToUniformSphereCapPdf(const Vector3f &v, Float cosThetaMax)
{
    if (v(2) < cosThetaMax)
        return 0.0f;
    return INV_TWOPI / (1.0f - cosThetaMax);
}

Vector3f WarpSpace::squareToUniformHemisphere(const Point2f &sample)
{
    Float cosTheta = 1.0 - sample(1); // TODO: verify this is ok
    Float sinTheta = std::sqrt(std::max((Float) 0, 1-cosTheta*cosTheta));

    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);

    return Vector3f(Float(cosPhi) * sinTheta, Float(sinPhi) * sinTheta, cosTheta);
}

Float WarpSpace::squareToUniformHemispherePdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_TWOPI : 0.0f;
}

Vector3f WarpSpace::squareToCosineHemisphere(const Point2f &sample)
{
    Point2f p = squareToUniformDisk(sample);
    Float z = std::sqrt(std::max((Float) 0, 1.0 - p(0)*p(0) - p(1)*p(1)));

    return Vector3f(p(0), p(1), z);
}

Float WarpSpace::squareToCosineHemispherePdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_PI * v(2) : 0.0f;
}

Vector3f WarpSpace::squareToCosinePowerHemisphere(const Point2f &sample, Float n)
{
    Float cosTheta  = std::pow(sample(1), 1.0f / (n + 1.0f));
    Float sinTheta  = std::sqrt(std::max((Float) 0.0f, 1.0f - cosTheta * cosTheta));

    Float sinPhi, cosPhi;
    feign_sincos(2.0 * M_PI * sample(0), &sinPhi, &cosPhi);

    return Vector3f(Float(cosPhi) * sinTheta, Float(sinPhi) * sinTheta, cosTheta);
}

Float WarpSpace::squareToCosinePowerHemispherePdf(const Vector3f &v, Float n)
{
    return v(2) > 0 ? 0.5f * (n + 1.0f) * std::pow(v(2), n) * INV_PI : 0.0f;
}

Point2f WarpSpace::squareToUniformTriangle(const Point2f &sample)
{
    Float u = 1 - sqrt(sample(0));
    Float v = sample(1) * sqrt(sample(0));

    return Point2f(u, v);
}

Float WarpSpace::squareToUniformTrianglePdf(const Point2f &sample)
{
    if (sample(0) + sample(1) > 1.0) return 0.0f;
    if (sample(0) + sample(1) < 0.0) return 0.0f;

    return 2.0f;
}

Vector3f WarpSpace::squareToBeckmann(const Point2f &sample, Float alpha)
{
    throw new NotImplementedException("square to beckmann");

    return Vector3f(0.f);
}

Float WarpSpace::squareToBeckmannPdf(const Vector3f &m, Float alpha)
{
    throw new NotImplementedException("square to beckmann pdf");

    return 0.f;
}
