#pragma once

#include <feign/node.h>

struct BSDFQuery
{
    Vector3f wi;
    Vector3f wo;

    Point3f uvw;

    Float eta;

    bool isDiffuse;

    BSDFQuery(const Vector3f& wi, Point2f& uv);
    BSDFQuery(const Vector3f& wi, Point3f& uvw);
    BSDFQuery(const Vector3f& wi, const Vector3f& wo, Point2f& uv);
    BSDFQuery(const Vector3f& wi, const Vector3f& wo, Point3f& uvw);
};

class BSDF : public Node
{
public:
    BSDF();
    BSDF(Node* parent);
    virtual ~BSDF();

    virtual Color3f sample(BSDFQuery& rec, const Point2f& sample) const = 0;
    virtual Color3f eval(const BSDFQuery& rec) const = 0;
    virtual Float pdf(const BSDFQuery& rec) const = 0;

    virtual bool isDelta() const = 0;

    virtual std::string getName() const;
    virtual NodeType getNodeType() const;

    // TODO
};
