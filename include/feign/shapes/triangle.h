/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

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
