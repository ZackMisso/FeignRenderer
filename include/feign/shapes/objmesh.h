#pragma once

#include <feign/shapes/shape.h>
#include <feign/math/transform.h>

class ObjMesh : public Shape
{
public:
    ObjMesh();
    ObjMesh(Node* parent);

    virtual string getName() const;

    uint32_t tris() const;
    uint32_t verts() const;

    float surfaceArea() const;
    float surfaceArea(uint32_t index) const;
    float pdf(uint32_t index) const;

    virtual bool intersect(const Ray3f& scene_ray, Intersection& its);
    bool intersect(uint32_t face, Ray3f& ray, Intersection& its);
    virtual void completeIntersectionInfo(const Ray3f& ray, Intersection& its) const;

    void parseFromFile(const string& filename, Transform transform, bool flipNorms);

    virtual void preProcess();

    void infoDump();

    // TODO
    // bool intersectTri(uint32_t index, const Ray& ray, *intersectionData*) const;

    const BBox3f& getBoundingBox() const;
    // TODO: bounding sphere
    const vector<Point3f>& getVerts() const;
    const vector<Normal3f>& getNorms() const;
    const vector<Vec3u>& getFaces() const;
    const vector<Vec2f>& getUVs() const;

protected:
    // note: this assumes a triangle mesh
    vector<Point3f> vs;
    vector<Normal3f> ns;
    vector<Vec2f> uvs;
    vector<Vec3u> fs;
    BBox3f bbox;
};
