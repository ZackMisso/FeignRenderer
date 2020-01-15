/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/sampler.h>

class WarpSpace
{
public:
    static Point2f squareToUniformDisk(const Point2f &sample);
    static Float squareToUniformDiskPdf(const Point2f &p);

    static Vector3f squareToUniformSphere(const Point2f &sample);
    static Float squareToUniformSpherePdf(const Vector3f &v);

    static Vector3f squareToUniformSphereCap(const Point2f &sample, Float cosThetaMax);
    static Float squareToUniformSphereCapPdf(const Vector3f &v, Float cosThetaMax);

    static Vector3f squareToUniformHemisphere(const Point2f &sample);
    static Float squareToUniformHemispherePdf(const Vector3f &v);

    static Vector3f squareToCosineHemisphere(const Point2f &sample);
    static Float squareToCosineHemispherePdf(const Vector3f &v);

    static Vector3f squareToCosinePowerHemisphere(const Point2f &sample, Float n);
    static Float squareToCosinePowerHemispherePdf(const Vector3f &v, Float n);

    static Point2f squareToUniformTriangle(const Point2f &sample);
    static Float squareToUniformTrianglePdf(const Point2f &sample);

    static Vector3f squareToBeckmann(const Point2f &sample, Float alpha);
    static Float squareToBeckmannPdf(const Vector3f &m, Float alpha);
};
