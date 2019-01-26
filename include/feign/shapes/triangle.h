#pragma once

#include <feign/common.h>
// #include <feign/math/vector.h>

// TODO: maybe create a separate abstraction for acceleration
class Triangle
{
public:
    Triangle(Vec3u vsInds,
             Vec3u nsInds,
             Vec3u uvsInds);

    Vec3u getVertInds() const;
    Vec3u getNormInds() const;
    Vec3u getUvInds() const;

    Vec3u vsInds;
    Vec3u nsInds;
    Vec3u uvsInds;
};
