/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/bsdfs/bsdf.h>

BSDFQuery::BSDFQuery(const Vector3f& wi,
                     const Point2f& uv,
                     const Point3f& pos)
    : wi(wi), uvw(Point3f(uv, 0.f)), pos(pos) { }

BSDFQuery::BSDFQuery(const Vector3f& wi,
                     const Point3f& uvw,
                     const Point3f& pos)
    : wi(wi), uvw(uvw), pos(pos) { }

BSDFQuery::BSDFQuery(const Vector3f& wi,
                     const Vector3f& wo,
                     const Point2f& uv,
                     const Point3f& pos)
    : wi(wi), wo(wo), uvw(Point3f(uv, 0.f)), pos(pos) { }

BSDFQuery::BSDFQuery(const Vector3f& wi,
                     const Vector3f& wo,
                     const Point3f& uvw,
                     const Point3f& pos)
    : wi(wi), wo(wo), uvw(uvw), pos(pos) { }


BSDF::BSDF() : Node() { }

BSDF::BSDF(Node* parent) : Node(parent) { }

BSDF::~BSDF()
{
    std::cout << "deleting bsdf" << std::endl;
    // does nothing
}

std::string BSDF::getName() const
{
    return "bsdf: ";
}

NodeType BSDF::getNodeType() const
{
    return NT_BSDF;
}
