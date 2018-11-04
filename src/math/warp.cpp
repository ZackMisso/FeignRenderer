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
    float r = std::sqrt(1-sample(1));
    float sinPhi, cosPhi;
    sincosf(2.0f * M_PI * sample(0), &sinPhi, &cosPhi);

    return Point2f(cosPhi * r, sinPhi * r);
}

float WarpSpace::squareToUniformDiskPdf(const Point2f &p)
{
    return p.sqrNorm() <= 1 ? INV_PI : 0.0f;
}

Vector3f WarpSpace::squareToUniformSphere(const Point2f &sample)
{
    float z = 1.0f - 2.0f * sample(1);
    float r = std::sqrt(std::max((float) 0.0f, 1.0f - z*z));
    float sinPhi, cosPhi;
    sincosf(2.0f * M_PI * sample(0), &sinPhi, &cosPhi);
    return Vector3f(r * cosPhi, r * sinPhi, z);
}

float WarpSpace::squareToUniformSpherePdf(const Vector3f &v)
{
    return INV_FOURPI;
}

Vector3f WarpSpace::squareToUniformSphereCap(const Point2f &sample, float cosThetaMax)
{
    float z = sample(1) + (1.f-sample(1)) * cosThetaMax;
    float r = std::sqrt(1-z*z);
    float sinPhi, cosPhi;
    sincosf(2.0f * M_PI * sample(0), &sinPhi, &cosPhi);
    return Vector3f(r * cosPhi, r * sinPhi, z);
}

float WarpSpace::squareToUniformSphereCapPdf(const Vector3f &v, float cosThetaMax)
{
    if (v(2) < cosThetaMax)
        return 0.0f;
    return INV_TWOPI / (1.0f - cosThetaMax);
}

Vector3f WarpSpace::squareToUniformHemisphere(const Point2f &sample)
{
    float cosTheta = sample(1);
    float sinTheta = std::sqrt(std::max((float) 0, 1-cosTheta*cosTheta));

    float sinPhi, cosPhi;
    sincosf(2.0f * M_PI * sample(0), &sinPhi, &cosPhi);

    return Vector3f(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta);
}

float WarpSpace::squareToUniformHemispherePdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_TWOPI : 0.0f;
}

Vector3f WarpSpace::squareToCosineHemisphere(const Point2f &sample)
{
    Point2f p = squareToUniformDisk(sample);
    float z = std::sqrt(std::max((float) 0, 1.0f - p(0)*p(0) - p(1)*p(1)));

    return Vector3f(p(0), p(1), z);
}

float WarpSpace::squareToCosineHemispherePdf(const Vector3f &v)
{
    return v(2) > 0 ? INV_PI * v(2) : 0.0f;
}

Vector3f WarpSpace::squareToCosinePowerHemisphere(const Point2f &sample, float n)
{
    float cosTheta  = std::pow(sample(1), 1.0f / (n + 1.0f));
    float sinTheta  = std::sqrt(std::max((float) 0.0f, 1.0f - cosTheta * cosTheta));

    float sinPhi, cosPhi;
    sincosf(2.0f * M_PI * sample(0), &sinPhi, &cosPhi);

    return Vector3f(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta);
}

float WarpSpace::squareToCosinePowerHemispherePdf(const Vector3f &v, float n)
{
    return v(2) > 0 ? 0.5f * (n + 1.0f) * std::pow(v(2), n) * INV_PI : 0.0f;
}

Point2f WarpSpace::squareToUniformTriangle(const Point2f &sample)
{
    float u = 1 - sqrt(sample(0));
    float v = sample(1) * sqrt(sample(0));

    return Point2f(u, v);
}

float WarpSpace::squareToUniformTrianglePdf(const Point2f &sample)
{
    if (sample(0) + sample(1) > 1.0) return 0.0f;
    if (sample(0) + sample(1) < 0.0) return 0.0f;

    return 2.0f;
}

Vector3f WarpSpace::squareToBeckmann(const Point2f &sample, float alpha)
{
    throw new NotImplementedException("square to beckmann");

    return Vector3f(0.f);
}

float WarpSpace::squareToBeckmannPdf(const Vector3f &m, float alpha)
{
    throw new NotImplementedException("square to beckmann pdf");

    return 0.f;
}
