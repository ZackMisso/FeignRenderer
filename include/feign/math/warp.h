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
    static Point2f sqrToUniDisk(const Point2f &sample);
    static Float sqrToUniDiskPdf(const Point2f &p);

    static Vector3f sqrToUniSph(const Point2f &sample);
    static Float sqrToUniSphPdf(const Vector3f &v);

    static Vector3f sqrToUniSphCap(const Point2f &sample, Float cosThetaMax);
    static Float sqrToUniSphCapPdf(const Vector3f &v, Float cosThetaMax);

    static Vector3f sqrToUniHemi(const Point2f &sample);
    static Float sqrToUniHemiPdf(const Vector3f &v);

    static Vector3f sqrToCosHemi(const Point2f &sample);
    static Float sqrToCosHemiPdf(const Vector3f &v);

    static Vector3f sqrToCosPowHemi(const Point2f &sample, Float n);
    static Float sqrToCosPowHemiPdf(const Vector3f &v, Float n);

    static Point2f sqrToUniTri(const Point2f &sample);
    static Float sqrToUniTriPdf(const Point2f &sample);

    static Vector3f sqrToBeck(const Point2f &sample, Float alpha);
    static Float sqrToBeckPdf(const Vector3f &m, Float alpha);
};
