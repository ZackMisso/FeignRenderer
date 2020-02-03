/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/shape.h>
#include <feign/math/discrete_pdf.h>

// TODO: is this the best way of incorporating emitters
/////////////////////////////////////////////////
// Emitter Query Object
/////////////////////////////////////////////////
struct EmitterQuery
{
    Vector3f p;
    Vector3f wi;
    Vector3f sh_n;
    // Vector3f g_n;
    // Point2f uv;
    Float sqr_dist;
    // Float pdf;
    // int prim_index;
    // int em_index;

    EmitterQuery(const Vector3f& p) : p(p) { }
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Emitter
/////////////////////////////////////////////////
class Emitter
{
public:
    Emitter() { }
    virtual ~Emitter() { }

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const = 0;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const = 0;

    virtual Color3f evaluate(EmitterQuery& rec) const = 0;

    virtual void preProcess() { }

    // this is bad design, figure out a better way of supporting
    // mesh emitters
    virtual void setMeshNode(MeshNode* node) { }
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Distant Emitter
/////////////////////////////////////////////////
class DistantEmitter : public Emitter
{
public:
    DistantEmitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void preProcess();

protected:
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Directional Emitter
/////////////////////////////////////////////////
class DirectionalEmitter : public Emitter
{
public:
    DirectionalEmitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void preProcess();

protected:
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Point Emitter
/////////////////////////////////////////////////
class PointEmitter : public Emitter
{
public:
    struct Params
    {
        Params(Color3f intensity, Point3f pos)
            : intensity(intensity), pos(pos) { }

        Color3f intensity;
        Point3f pos;
    };

    PointEmitter();
    PointEmitter(Color3f I,
                 Point3f pos);

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void preProcess();

protected:
    Color3f I;
    Point3f pos;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Inter Environment Emitter
/////////////////////////////////////////////////
// simply interpolates between two colors, based on the up direction
// of the ray
/////////////////////////////////////////////////
class InterpEnvironmentEmitter : public Emitter
{
public:
    struct Params
    {
        Params(Color3f top, Color3f bot)
            : top(top), bot(bot) { }

        Color3f top;
        Color3f bot;
    };

    InterpEnvironmentEmitter(Color3f top,
                             Color3f bot);

    // environment emitters should not be sampled
    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

protected:
    Color3f top;
    Color3f bot;
};

// TODO: this needs a transform
/////////////////////////////////////////////////
// Mesh Emitter
/////////////////////////////////////////////////
class MeshEmitter : public Emitter
{
public:
    struct Params
    {
        Params(Color3f intensity)
            : intensity(intensity) { }

        Color3f intensity;
    };

    MeshEmitter(Color3f intensity);
    ~MeshEmitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void setMeshNode(MeshNode* node);

    virtual void preProcess();

protected:
    MeshNode* mesh;
    DiscretePDF1D* sa_pdf;
    Color3f intensity;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Environment Emitter
/////////////////////////////////////////////////
class EnvironmentEmitter : public Emitter
{
public:
    EnvironmentEmitter();

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void preProcess();

protected:
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Emittrer Node structure
/////////////////////////////////////////////////
struct EmitterNode : public Node
{
public:
    EmitterNode()
        : emitter(nullptr),
          objectNode(nullptr) { }
    EmitterNode(std::string name)
        : Node(name),
          emitter(nullptr),
          objectNode(nullptr) { }
    EmitterNode(Emitter* emitter)
        : emitter(emitter),
          objectNode(nullptr) { }

    ~EmitterNode() { delete emitter; }

    ObjectNode* objectNode;
    Emitter* emitter;
};
/////////////////////////////////////////////////
