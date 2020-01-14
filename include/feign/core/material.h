/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/bsdf.h>
#include <feign/misc/intersection.h>

class Material
{
public:
    Material() { }
    virtual ~Material() { }

    // should this get the bsdf???
    // need to do more research on how more complicated materials work
    virtual BSDF* getBSDF(const Intersection& its,
                          Color3f& mat_scale) const = 0;
};

// a material with only one bsdf
class SimpleMaterial : public Material
{
public:
    struct Params
    {
        Params(std::string bsdf_name)
            : bsdf_name(bsdf_name) { }

        std::string bsdf_name;
    };

    SimpleMaterial(BSDFNode* bsdf);
    ~SimpleMaterial();

    virtual BSDF* getBSDF(const Intersection& its,
                          Color3f& mat_scale) const;

    BSDFNode* bsdf;
};

// TODO: convert this to a shader
// a material with different scattering properties along the edges of a mesh
class WireframeMaterial : public Material
{
public:
    struct Params
    {
        Params(std::string wireframe_bsdf,
               std::string mesh_bsdf,
               float threshold)
            : wireframe_bsdf(wireframe_bsdf),
              mesh_bsdf(mesh_bsdf),
              threshold(threshold) { }

        std::string wireframe_bsdf;
        std::string mesh_bsdf;
        float threshold;
    };

    WireframeMaterial(BSDFNode* wireframe_bsdf,
                      BSDFNode* mesh_bsdf,
                      float threshold);
    ~WireframeMaterial();

    virtual BSDF* getBSDF(const Intersection& its,
                          Color3f& mat_scale) const;

    BSDFNode* wireframe_bsdf;
    BSDFNode* mesh_bsdf;
    float threshold;
};

// TODO: convert this to a shader
// a material which creates a radar like effect across a surface based
// on the distance a point is to that point. It has a linear fall off
// initially
class RadarMaterial : public Material
{
public:
    struct Params
    {
        Params(std::string radar_bsdf,
               std::string mesh_bsdf,
               std::vector<Point3f> start_points,
               std::vector<Float> end_dist,
               std::vector<Float> start_times,
               std::vector<Float> end_times,
               float band_width,
               float fall_off,
               float proxy)
            : radar_bsdf(radar_bsdf),
              mesh_bsdf(mesh_bsdf),
              start_points(start_points),
              end_dist(end_dist),
              start_times(start_times),
              end_times(end_times),
              band_width(band_width),
              fall_off(fall_off),
              proxy(proxy) { }

        std::string radar_bsdf;
        std::string mesh_bsdf;
        std::vector<Point3f> start_points;
        std::vector<Float> end_dist;
        std::vector<Float> start_times;
        std::vector<Float> end_times;
        float band_width;
        float fall_off;
        float proxy;
    };

    RadarMaterial(BSDFNode* radar_bsdf,
                  BSDFNode* mesh_bsdf,
                  std::vector<Point3f> start_points,
                  std::vector<Float> end_dist,
                  std::vector<Float> start_proxy,
                  std::vector<Float> end_proxy,
                  float band_width,
                  float fall_off,
                  float proxy);

    virtual BSDF* getBSDF(const Intersection& its,
                          Color3f& mat_scale) const;

    BSDFNode* radar_bsdf;
    BSDFNode* mesh_bsdf;
    std::vector<Point3f> start_points;
    std::vector<Float> end_dist;
    std::vector<Float> start_times;
    std::vector<Float> end_times;
    float band_width;
    float fall_off;
    float proxy;
};

/////////////////////////////////////////////////
// Material Node structure
/////////////////////////////////////////////////
struct MaterialNode : public Node
{
public:
    MaterialNode() : material(nullptr) { }
    MaterialNode(std::string name) : Node(name), material(nullptr) { }
    MaterialNode(Material* material) : material(material) { }

    ~MaterialNode() { delete material; }

    Material* material;
};
/////////////////////////////////////////////////
