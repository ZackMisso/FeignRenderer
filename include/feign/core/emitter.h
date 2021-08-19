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
#include <feign/core/texture.h>
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
    EmitterQuery(const Vector3f &p) : p(p) {}

    // this constructor is used for light tracing / photon mapping
    EmitterQuery() {}
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Emitter
/////////////////////////////////////////////////
class Emitter
{
public:
    Emitter() {}
    virtual ~Emitter() {}

    // samples a ray starting from the emitter (for light tracing / photon mapping)
    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const = 0;

    // will eventually be used for euiangular sampling along paths through a medium
    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const = 0;

    // samples a position on the light source for next event estimation
    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const = 0;

    // TODO: is this ever used? Will also have to rethink how to organize this
    //       since the emitterquery will now be slightly different depending
    //       on if the ray is going from the camera->emitter or emitter->camera.
    virtual Color3f evaluate(EmitterQuery &rec) const = 0;

    virtual bool isSpatial() const = 0;
    virtual Point3f getCenter() const = 0;

    // TODO: this method currently exists for supporting the directional mesh
    //       emitter, but really it appears more of a hack than anything. Need
    //       to figure out a better way of doing this without this unnecessary
    //       functionality.
    //
    // this method is currently used to indicate wbether or not an emitter needs
    // an additional visibility check after sampling it for next event estimation.
    // this is used for emitters which only emit line in singular directions where
    // sampling any random direction would typically result in a rejected sample.
    virtual bool requiresInitialVisibilityCheck() const { return false; }

    virtual bool isEnvironmentOnlyEmitter() const { return false; }
    virtual bool isEnvironmentEmitter() const { return false; }

    virtual void preProcess() {}

    // this is bad design, figure out a better way of supporting
    // mesh emitters
    virtual void setMeshNode(MeshNode *node) {}

    // if a ray hits an emitter, then that emitter has a mesh. If that emitter is
    // hit and also has a BSDF, then the emitter should not be sampled during
    // next event estimation. An easier way of accomplishing this is forcing the
    // nee result to be zero rather than modifying all of the light selection
    // algorithms. For this to work, mesh emitters need to be able to expose their
    // meshes to verify whether or not the intersection is actually with the
    // emitter to begin with.
    virtual MeshNode *getMeshNode() { return nullptr; }
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Distant Emitter
/////////////////////////////////////////////////
class DistantEmitter : public Emitter
{
public:
    DistantEmitter();

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

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
            : light_dir(light_dir), radiance(radiance) {}

        Vector3f light_dir;
        Color3f radiance;
    };

    DirectionalEmitter(Vector3f light_dir, Color3f radiance);

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

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
              light_angle(light_angle) {}

        Point3f light_pos;
        Vector3f light_dir;
        Color3f radiance;
        Float light_angle;
    };

    SpotLightEmitter(Point3f light_pos,
                     Vector3f light_dir,
                     Color3f radiance,
                     Float light_angle);

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

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
            : intensity(intensity), pos(pos) {}

        Color3f intensity;
        Point3f pos;
    };

    PointEmitter();
    PointEmitter(Color3f I,
                 Point3f pos);

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

    virtual void preProcess();

    virtual bool isSpatial() const { return true; }
    virtual Point3f getCenter() const;

protected:
    Color3f I; // TODO: this is actually storing power
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
            : top(top), bot(bot) {}

        Color3f top;
        Color3f bot;
    };

    InterpEnvironmentEmitter(Color3f top,
                             Color3f bot);

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    // environment emitters should not be sampled
    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

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
            : intensity(intensity) {}

        Color3f intensity;
    };

    MeshEmitter(Color3f intensity);
    ~MeshEmitter();

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

    virtual void setMeshNode(MeshNode *node);

    virtual void preProcess();

    virtual bool isSpatial() const { return true; }
    virtual Point3f getCenter() const;

    virtual MeshNode *getMeshNode() { return mesh; }

protected:
    MeshNode *mesh;
    DiscretePDF1D *sa_pdf;
    Color3f intensity;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Directional Emitter
/////////////////////////////////////////////////
// a mesh emitter is a diffuse light source, where as this emitter only emits
// light in one direction. TODO: make a generalized emitter based on arbitrary
// reflectance distributions.
class DirectionalMeshEmitter : public Emitter
{
public:
    struct Params
    {
        Params(Vector3f light_dir, Color3f intensity)
            : light_dir(light_dir), intensity(intensity) {}

        Vector3f light_dir;
        Color3f intensity;
    };

    DirectionalMeshEmitter(Vector3f light_dir, Color3f intensity);
    ~DirectionalMeshEmitter();

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

    virtual void setMeshNode(MeshNode *node);
    virtual void preProcess();

    virtual bool isSpatial() const { return true; }
    virtual bool requiresInitialVisibilityCheck() const { return true; }
    virtual Point3f getCenter() const;

    virtual MeshNode *getMeshNode() { return mesh; }

protected:
    MeshNode *mesh;
    DiscretePDF1D *sa_pdf;
    Vector3f light_dir;
    Color3f intensity;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Environment Emitter
// TODO: implement support for importance sampling
//       Note: will only allow support if the texture is non-procedural...
/////////////////////////////////////////////////
class EnvironmentEmitter : public Emitter
{
public:
    struct Params
    {
        Params(Color3f intensity, std::string texture)
            : intensity(intensity), texture(texture) {}

        Color3f intensity;
        std::string texture;
    };

    EnvironmentEmitter(TextureNode *texture, Color3f intensity);

    virtual Color3f sample_ray(EmitterQuery &rec,
                               const Point2f &dir_sample,
                               const Point2f &point_sample,
                               Float *pdf) const;

    virtual Color3f sample_medium(EmitterQuery &rec,
                                  const Point2f &sample,
                                  Float *pdf) const;

    virtual Color3f sample_nee(EmitterQuery &rec,
                               const Point2f &sample,
                               Float *pdf) const;

    virtual Color3f evaluate(EmitterQuery &rec) const;

    virtual bool isSpatial() const { return false; }
    virtual bool requiresInitialVisibilityCheck() const { return false; }
    virtual bool isEnvironmentOnlyEmitter() const { return true; }
    virtual bool isEnvironmentEmitter() const { return true; }
    virtual Point3f getCenter() const { return Point3f(0.f); }

    virtual void preProcess();

protected:
    TextureNode *texture;
    Color3f intensity;
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
          objectNode(nullptr) {}
    EmitterNode(std::string name)
        : Node(name),
          emitter(nullptr),
          objectNode(nullptr) {}
    EmitterNode(Emitter *emitter)
        : emitter(emitter),
          objectNode(nullptr) {}

    ~EmitterNode() { delete emitter; }

    ObjectNode *objectNode;
    Emitter *emitter;
};
/////////////////////////////////////////////////

FEIGN_END()
