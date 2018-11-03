#pragma once

#include <feign/common.h>
#include <feign/samplers/sampler.h>

class WarpSpace
{
public:
    static Vector3f sampleUniformHemisphere(Sampler *sampler, const Normal3f& northPole);

    static Point2f squareToUniformDisk(const Point2f &sample);
    static float squareToUniformDiskPdf(const Point2f &p);

    static Vector3f squareToUniformSphere(const Point2f &sample);
    static float squareToUniformSpherePdf(const Vector3f &v);

    static Vector3f squareToUniformSphereCap(const Point2f &sample, float cosThetaMax);
    static float squareToUniformSphereCapPdf(const Vector3f &v, float cosThetaMax);

    static Vector3f squareToUniformHemisphere(const Point2f &sample);
    static float squareToUniformHemispherePdf(const Vector3f &v);

    static Vector3f squareToCosineHemisphere(const Point2f &sample);
    static float squareToCosineHemispherePdf(const Vector3f &v);

    static Vector3f squareToCosinePowerHemisphere(const Point2f &sample, float n);
    static float squareToCosinePowerHemispherePdf(const Vector3f &v, float n);

    static Point2f squareToUniformTriangle(const Point2f &sample);
    static float squareToUniformTrianglePdf(const Point2f &sample);

    static Vector3f squareToBeckmann(const Point2f &sample, float alpha);
    static float squareToBeckmannPdf(const Vector3f &m, float alpha);
};
