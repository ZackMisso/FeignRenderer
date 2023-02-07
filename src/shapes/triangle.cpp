/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/shapes/triangle.h>

FEIGN_BEGIN()

Triangle::Triangle(Vec3u vsInds,
                   Vec3u nsInds,
                   Vec3u uvsInds)
    : vsInds(vsInds), nsInds(nsInds), uvsInds(uvsInds)
{
}

Vec3u Triangle::getVertInds() const { return vsInds; }
Vec3u Triangle::getNormInds() const { return nsInds; }
Vec3u Triangle::getUvInds() const { return uvsInds; }

FEIGN_END()
