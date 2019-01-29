#pragma once

#include <feign/shapes/shape.h>
#include <feign/shapes/triangle.h>
#include <feign/math/transform.h>

class ObjMesh : public Shape
{
public:
    ObjMesh();
    ObjMesh(Node* parent);
    ObjMesh(const std::vector<Point3f>& vs,
            const std::vector<Normal3f>& ns,
            const std::vector<Vec2f>& uvs,
            const std::vector<Triangle>& tris);

    virtual std::string getName() const;

    uint32_t num_tris() const;
    uint32_t num_verts() const;

    float surfaceArea() const;
    float surfaceArea(uint32_t index) const;
    float pdf(uint32_t index) const;

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its) const;
    bool intersect(uint32_t tri, const Ray3f& ray, Intersection& its) const;

    virtual void completeIntersectionInfo(const Ray3f& ray, Intersection& its) const;

    void parseFromFile(const std::string& filename,
                       Transform transform,
                       bool flipNorms);

    virtual void preProcess();

    void infoDump();

    const BBox3f& getBoundingBox() const;
    // TODO: bounding sphere
    const std::vector<Point3f>& getVerts() const;
    const std::vector<Normal3f>& getNorms() const;
    const std::vector<Vec2f>& getUVs() const;

protected:
    // note: this assumes a triangle mesh
    std::vector<Triangle> tris;
    std::vector<Point3f> vs;
    std::vector<Normal3f> ns;
    std::vector<Vec2f> uvs;

    BBox3f bbox;
};
