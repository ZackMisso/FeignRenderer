#include <feign/bsdfs/bsdf.h>

BSDFQuery::BSDFQuery(const Vector3f& wi, Point2f& uv)
    : wi(wi), uvw(Point3f(uv, 0.f)) { }

BSDFQuery::BSDFQuery(const Vector3f& wi, Point3f& uvw)
    : wi(wi), uvw(uvw) { }

BSDFQuery::BSDFQuery(const Vector3f& wi, const Vector3f& wo, Point2f& uv)
    : wi(wi), wo(wo), uvw(Point3f(uv, 0.f)) { }

BSDFQuery::BSDFQuery(const Vector3f& wi, const Vector3f& wo, Point3f& uvw)
    : wi(wi), wo(wo), uvw(uvw) { }


BSDF::BSDF() : Node() { }

BSDF::BSDF(Node* parent) : Node(parent) { }

BSDF::~BSDF()
{
    // does nothing
}

string BSDF::getName() const
{
    return "bsdf: ";
}

NodeType BSDF::getNodeType() const
{
    return NT_BSDF;
}
