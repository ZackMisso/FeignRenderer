#include <feign/shapes/triangle.h>

Triangle::Triangle(Vec3u vsInds,
                   Vec3u nsInds,
                   Vec3u uvsInds)
    : vsInds(vsInds), nsInds(nsInds), uvsInds(uvsInds)
{
}

Vec3u Triangle::getVertInds() const { return vsInds; }
Vec3u Triangle::getNormInds() const { return nsInds; }
Vec3u Triangle::getUvInds() const { return uvsInds; }
