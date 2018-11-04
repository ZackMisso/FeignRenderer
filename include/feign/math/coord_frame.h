// This code is based off of Wenzel Jakob's implementation in Nori

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
    CoordinateFrame(const Vector3f& n);

    Vector3f toLocal(const Vector3f& vec) const;
    Vector3f toWorld(const Vector3f& vec) const;

    void coordinateSystem(const Vector3f &a, Vector3f &b, Vector3f &c);

    static float cosTheta(const Vector3f& vec);
    static float sinTheta(const Vector3f& vec);
    static float tanTheta(const Vector3f& vec);
    static float sinThetaSqr(const Vector3f& vec);
    static float sinPhi(const Vector3f& vec);
    static float cosPhi(const Vector3f& vec);
    static float sinPhiSqr(const Vector3f& vec);
    static float cosPhiSqr(const Vector3f& vec);
};
