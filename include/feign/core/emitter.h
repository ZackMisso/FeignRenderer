/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/shape.h>
#include <feign/math/vector.h>
#include <feign/math/discrete_pdf.h>

FEIGN_BEGIN()

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

    // this constructor is used for when the emitter is being quaried from a point
    EmitterQuery(const Vector3f& p) : p(p) { }

    // this constructor is used for light tracing / photon mapping
    EmitterQuery() { }
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

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const = 0;

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const = 0;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const = 0;

    virtual Color3f evaluate(EmitterQuery& rec) const = 0;

    virtual bool isSpatial() const = 0;
    virtual Point3f getCenter() const = 0;

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

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const;

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual Point3f getCenter() const;

    virtual bool isSpatial() const { return false; }
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
    struct Params
    {
        Params(Vector3f light_dir, Color3f radiance)
            : light_dir(light_dir), radiance(radiance) { }

        Vector3f light_dir;
        Color3f radiance;
    };

    DirectionalEmitter(Vector3f light_dir, Color3f radiance);

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const;

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual bool isSpatial() const { return false; }
    virtual Point3f getCenter() const { return Point3f(0.f); }

protected:
    Vector3f light_dir;
    Color3f radiance;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// SpotLight Emitter
/////////////////////////////////////////////////
class SpotLightEmitter : public Emitter
{
public:
    struct Params
    {
        Params(Point3f light_pos,
               Vector3f light_dir,
               Color3f radiance,
               Float light_angle)
            : light_pos(light_pos),
              light_dir(light_dir),
              radiance(radiance),
              light_angle(light_angle) { }

        Point3f light_pos;
        Vector3f light_dir;
        Color3f radiance;
        Float light_angle;
    };

    SpotLightEmitter(Point3f light_pos,
                     Vector3f light_dir,
                     Color3f radiance,
                     Float light_angle);

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const;

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual bool isSpatial() const { return true; }
    virtual Point3f getCenter() const;

protected:
    Point3f light_pos;
    Vector3f light_dir;
    Color3f radiance;
    Float light_angle;
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

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const;

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void preProcess();

    virtual bool isSpatial() const { return true; }
    virtual Point3f getCenter() const;

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

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const;

    // environment emitters should not be sampled
    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual bool isSpatial() const { return false; }
    virtual Point3f getCenter() const { return Point3f(0.f); }

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

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const;

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void setMeshNode(MeshNode* node);

    virtual void preProcess();

    virtual bool isSpatial() const { return true; }
    virtual Point3f getCenter() const;

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

    virtual Color3f sample_ray(EmitterQuery& rec,
                               const Point2f& dir_sample,
                               const Point2f& point_sample,
                               Float* pdf) const;

    virtual Color3f sample_li(EmitterQuery& rec,
                              const Point2f& sample,
                              Float* pdf) const;

    virtual Color3f sample_pos(EmitterQuery& rec,
                               const Point2f& sample,
                               Float* pdf) const;

    virtual Color3f evaluate(EmitterQuery& rec) const;

    virtual void preProcess();

    virtual bool isSpatial() const { return false; }
    virtual Point3f getCenter() const { return Point3f(0.f); }

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

FEIGN_END()
