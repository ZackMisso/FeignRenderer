/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
// #include <feign/core/material.h>
#include <feign/core/bsdf.h>
#include <feign/math/vector.h>
#include <feign/math/functor.h>

FEIGN_BEGIN()

// material shaders manipulate and control the actual materials which are on objects.
// These shaders are also the entry point for the information which integrators
// get while evaluating materials.
struct MaterialShader
{
public:
    virtual ~MaterialShader() {}

    virtual void sample(MaterialClosure &closure) const = 0;
    virtual void evaluate(MaterialClosure &closure) const = 0;
    virtual void evaluate_mat_only(MaterialClosure &closure) const = 0;
    virtual void evaluate_for_photon(MaterialClosure &closure) const = 0;
};

// a simple material shader which just evaluates one material
struct SimpleMaterialShader : public MaterialShader
{
    struct Params
    {
        Params(std::string bsdf)
            : bsdf(bsdf) {}

        std::string bsdf;
    };

    SimpleMaterialShader(BSDFNode *bsdf);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;
    virtual void evaluate_mat_only(MaterialClosure &closure) const;
    virtual void evaluate_for_photon(MaterialClosure &closure) const;

    BSDFNode *bsdf;
};

// this will take some heavy re-writing probably.
struct WireframeMaterialShader : public MaterialShader
{
public:
    struct Params
    {
        Params(std::string wireframe_mat,
               std::string mesh_mat,
               float threshold)
            : wireframe_mat(wireframe_mat),
              mesh_mat(mesh_mat),
              threshold(threshold) {}

        std::string wireframe_mat;
        std::string mesh_mat;
        float threshold;
    };

    WireframeMaterialShader(BSDFNode *wireframe_bsdf,
                            BSDFNode *mesh_bsdf,
                            float threshold);

    virtual void sample(MaterialClosure &closure) const;
    virtual void evaluate(MaterialClosure &closure) const;
    virtual void evaluate_mat_only(MaterialClosure &closure) const;
    virtual void evaluate_for_photon(MaterialClosure &closure) const;

    BSDF *choose_bsdf(MaterialClosure &closure) const;

    BSDFNode *wireframe_bsdf;
    BSDFNode *mesh_bsdf;
    float threshold;
};

// this is very meh now. I only used this for a test of the actual
// shader framework and I do not think this should really be incorporated
// in the renderer anymore, TODO: remove this later
// struct RadarMaterialShader : public MaterialShader
// {
//     struct Params
//     {
//         Params(std::string radar_mat,
//                std::string mesh_mat,
//                std::vector<Point3f> start_points,
//                std::vector<Float> end_dist,
//                std::vector<Float> start_times,
//                std::vector<Float> end_times,
//                float band_width,
//                float fall_off,
//                float proxy)
//             : radar_mat(radar_mat),
//               mesh_mat(mesh_mat),
//               start_points(start_points),
//               end_dist(end_dist),
//               start_times(start_times),
//               end_times(end_times),
//               band_width(band_width),
//               fall_off(fall_off),
//               proxy(proxy) { }
//
//         std::string radar_mat;
//         std::string mesh_mat;
//         std::vector<Point3f> start_points;
//         std::vector<Float> end_dist;
//         std::vector<Float> start_times;
//         std::vector<Float> end_times;
//         float band_width;
//         float fall_off;
//         float proxy;
//     };
//
//     RadarMaterialShader(MaterialNode* radar_mat,
//                         MaterialNode* mesh_mat,
//                         std::vector<Point3f> start_points,
//                         std::vector<Float> end_dist,
//                         std::vector<Float> start_proxies,
//                         std::vector<Float> end_proxies,
//                         float band_width,
//                         float fall_off,
//                         float proxy);
//
//     virtual void sample(MaterialClosure& closure) const;
//     virtual void evaluate(MaterialClosure& closure) const;
//     virtual void evaluate_mat_only(MaterialClosure& closure) const;
//     virtual void evaluate_for_photon(MaterialClosure& closure) const;
//     Material* choose_mat(MaterialClosure& closure, Color3f& scale) const;
//
//     MaterialNode* radar_mat;
//     MaterialNode* mesh_mat;
//     std::vector<Point3f> start_points;
//     std::vector<Float> end_dist;
//     std::vector<Float> start_proxies;
//     std::vector<Float> end_proxies;
//     float band_width;
//     float fall_off;
//     float proxy;
// };

// geometry shaders manipulate the vertices, normals, and triangles directly.
// These shaders do not manipulate the lighting but instead the physical locations
// and directions of these quantities.
struct GeometryShader
{
public:
    virtual ~GeometryShader() {}

    // TODO: figure out a way to make this safe
    virtual void evaluate(void *mesh) {}
    virtual bool isValid(MeshType mesh_type) const { return true; }
};

// this shader interpolates the geometry of an objmesh between its actual mesh
// and a projection of its mesh onto a sphere. The sphere will have a radius
// equal to the radius of the shortest axis of its bounding box multiplied by
// the prop_of_shortest_axis parameter with its center located at the centroid
// of the mesh
struct InterpVertsToSphereShader : public GeometryShader
{
public:
    struct Params
    {
        Params(float prop, float proxy)
            : prop(prop), proxy(proxy) {}

        float prop;
        float proxy;
    };

    InterpVertsToSphereShader(float prop_of_shortest_axis, float interp);

    virtual void evaluate(void *mesh);
    virtual bool isValid(MeshType mesh_type) const;

    float prop_of_shortest_axis;
    float interp;
};

// TODO: fix this and make it work: it could be cool
// used with grid objects to make wave displacements
// struct WaveDisplacementShader : public GeometryShader
// {
// public:
//     struct Params
//     {
//         // TODO: create functor abstaction later
//         //       for now this just stores max value
//         Params(std::vector<Point2f> points,
//                std::vector<Functor*> functors,
//                std::vector<Float> start_proxies,
//                std::vector<Float> end_proxies)
//             : points(points),
//               functors(functors),
//               start_proxies(start_proxies),
//               end_proxies(end_proxies) { }
//
//         std::vector<Point2f> points;
//         std::vector<Functor*> functors;
//         std::vector<Float> start_proxies;
//         std::vector<Float> end_proxies;
//     };
//
//     WaveDisplacementShader(std::vector<Point2f> points,
//                            std::vector<Functor> functors,
//                            std::vector<Float> start_proxies,
//                            std::vector<Float> end_proxies);
//
//     virtual void evaluate(void* mesh);
//     virtual bool isValid(MeshType mesh_type) const;
//
//     std::vector<Point2f> points;
//     std::vector<Float> functors;
//     std::vector<Float> start_proxies;
//     std::vector<Float> end_proxies;
// };

/////////////////////////////////////////////////
// Geometry Shader Node structure
/////////////////////////////////////////////////
struct GeometryShaderNode : public Node
{
public:
    GeometryShaderNode() : shader(nullptr) {}
    GeometryShaderNode(std::string name) : Node(name), shader(nullptr) {}

    ~GeometryShaderNode() { delete shader; }

    GeometryShader *shader;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Material Shader Node structure
/////////////////////////////////////////////////
struct MaterialShaderNode : public Node
{
public:
    MaterialShaderNode() : shader(nullptr) {}
    MaterialShaderNode(std::string name) : Node(name), shader(nullptr) {}

    ~MaterialShaderNode() { delete shader; }

    MaterialShader *operator()() { return shader; }

    MaterialShader *shader;
};
/////////////////////////////////////////////////

FEIGN_END()
