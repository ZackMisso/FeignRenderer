#pragma once

#include <feign/node.h>

struct EmitterQuery
{
    Vector3f p;
    Vector3f wi;
    Vector3f sh_n;
    Vector3f g_n;
    Point2f uv;
    Float sqr_dist;
    Float pdf;
    int prim_index;
    int em_index;

    EmitterQuery(const Vector3f& p) : p(p) { }
};

class Emitter : public Node
{
public:
    Emitter();
    Emitter(Node* parent);
    virtual ~Emitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const = 0;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const = 0;

    virtual std::string getName() const;
    virtual NodeType getNodeType() const;
};

// virtual Color3f sample(EmitterQueryRecord &rec, const Point2f &sample) const = 0;
//
// virtual float pdf(const EmitterQueryRecord &rec) const = 0;
//
// virtual Color3f eval(const EmitterQueryRecord &rec) const = 0;
