/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/shape.h>
#include <feign/shapes/triangle.h>
#include <feign/math/transform.h>

class ObjMesh : public Shape
{
public:
    struct Params
    {
        Params(std::string filename,
               std::string shader,
               bool flip_norms = false)
            : filename(filename),
              shader(shader),
              flip_norms(flip_norms) { }

        std::string filename;
        std::string shader;
        bool flip_norms;
    };

    ObjMesh();
    ObjMesh(const std::string& filename,
            bool flip_norms = false);
    ObjMesh(const std::vector<Point3f>& vs,
            const std::vector<Normal3f>& ns,
            const std::vector<Vec2f>& uvs,
            const std::vector<Triangle>& tris);

    ~ObjMesh() { }

    uint32_t num_tris() const;
    uint32_t num_verts() const;

    float surfaceArea() const;
    float surfaceArea(uint32_t index) const;
    float pdf(uint32_t index) const;

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;
    bool intersect(uint32_t tri, const Ray3f& ray, Intersection& its) const;

    virtual void completeIntersectionInfo(const Ray3f& ray, Intersection& its) const;

    virtual void addShapeToScene(RTCScene scene, RTCDevice device);

    void parseFromFile(const std::string& filename);

    virtual uint32_t primitiveCount() const;

    virtual BBox3f boundingBox() const;
    virtual BBox3f boundingBox(uint32_t tri) const;
    virtual Point3f centroid() const;
    virtual Point3f centroid(uint32_t tri) const;

    virtual void preProcess();

    void infoDump();

    // const BBox3f& getBoundingBox() const;
    // TODO: bounding sphere
    const std::vector<Triangle>& getTris() const;
    const std::vector<Point3f>& getVerts() const;
    const std::vector<Normal3f>& getNorms() const;
    const std::vector<Vec2f>& getUVs() const;

    void setVerts(std::vector<Point3f>& param);
    void setNorms(std::vector<Normal3f>& param);
    void setUVs(std::vector<Vec2f>& param);
protected:
    std::string filename;

    // note: this assumes a triangle mesh
    std::vector<Triangle> tris;
    std::vector<Point3f> vs;
    std::vector<Normal3f> ns;
    std::vector<Vec2f> uvs;

    BBox3f bbox;
    Float sa;
    bool flip_norms;
};
