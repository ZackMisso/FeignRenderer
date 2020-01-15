/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Based off Wenzel Jakob's implementation in Nori
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

struct CoordinateFrame
{
    Vector3f s;
    Vector3f t;
    Normal3f n;

    CoordinateFrame();
    CoordinateFrame(const Vector3f& s, const Vector3f& t, const Normal3f& n);
    CoordinateFrame(const Vector3f& x, const Vector3f& y, const Vector3f& z);
    CoordinateFrame(const Normal3f& n);

    Vector3f toLocal(const Vector3f& vec) const;
    Vector3f toWorld(const Vector3f& vec) const;

    static void coordinateSystem(const Vector3f &a, Vector3f &b, Vector3f &c);

    static Float cosTheta(const Vector3f& vec);
    static Float sinTheta(const Vector3f& vec);
    static Float tanTheta(const Vector3f& vec);
    static Float sinThetaSqr(const Vector3f& vec);
    static Float sinPhi(const Vector3f& vec);
    static Float cosPhi(const Vector3f& vec);
    static Float sinPhiSqr(const Vector3f& vec);
    static Float cosPhiSqr(const Vector3f& vec);
};
