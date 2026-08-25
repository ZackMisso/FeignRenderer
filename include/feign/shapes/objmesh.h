/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

// TODO: I also need to write a ply parser

#pragma once

#include <feign/core/shape.h>
#include <feign/shapes/triangle.h>
#include <feign/math/transform.h>
#include <feign/misc/medium_boundry.h>

FEIGN_BEGIN()

class ObjMesh : public Shape
{
public:
    struct Params
    {
        Params(std::string filename,
               std::string shader,
               bool flip_norms = false,
               std::string inside_media = "null",
               std::string outside_media = "null",
               bool is_null = false)
            : filename(filename),
              shader(shader),
              flip_norms(flip_norms),
              inside_media(inside_media),
              outside_media(outside_media),
              is_null(is_null) {}

        std::string inside_media;
        std::string outside_media;
        std::string filename;
        std::string shader;
        bool flip_norms;
        bool is_null;
    };

    ObjMesh();
    ObjMesh(const std::string &filename,
            bool flip_norms = false,
            const MediumBoundry *boundry = nullptr,
            bool is_null = false);
    ObjMesh(const std::vector<Point3f> &vs,
            const std::vector<Normal3f> &ns,
            const std::vector<Vec2f> &uvs,
            const std::vector<Triangle> &tris);

    ~ObjMesh() {}

    uint32_t num_tris() const;
    uint32_t num_verts() const;

    float surface_area() const;
    virtual Float surface_area(int primitive) const;
    float pdf(uint32_t index) const;

    virtual bool intersect(const Ray3f &scene_ray, Intersection &its) const;
    bool intersect(uint32_t tri, const Ray3f &ray, Intersection &its) const;

    virtual void complete_intersection_info(Intersection &its) const;

    virtual void add_shape_to_scene(RTCScene scene, RTCDevice device);

    void parse_from_file(const std::string &filename);

    virtual uint32_t primitive_count() const;

    virtual BBox3f bounding_box() const;
    virtual BBox3f bounding_box(uint32_t tri) const;
    virtual Point3f compute_centroid() const;
    virtual Point3f centroid() const;
    virtual Point3f centroid(uint32_t tri) const;

    virtual void pre_process(bool requires_processing = false);

    void info_dump();

    // const BBox3f& getBoundingBox() const;
    // TODO: bounding sphere
    const std::vector<Triangle> &get_tris() const;
    const std::vector<Point3f> &get_verts() const;
    const std::vector<Normal3f> &get_norms() const;
    const std::vector<Vec2f> &get_uvs() const;

    void set_verts(std::vector<Point3f> &param);
    void set_norms(std::vector<Normal3f> &param);
    void set_uvs(std::vector<Vec2f> &param);

protected:
    std::string filename;

    // note: this assumes a triangle mesh
    std::vector<Triangle> tris;
    std::vector<Point3f> vs;
    std::vector<Normal3f> ns;
    std::vector<Vec2f> uvs;

    BBox3f bbox;
    Point3f center;
    Float sa;
    bool flip_norms;
};

FEIGN_END()
