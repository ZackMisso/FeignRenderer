#pragma once

#include <feign/shapes/shape.h>

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

    // TODO
    // bool intersectTri(uint32_t index, const Ray& ray, *intersectionData*) const;

    const BBox3f& getBoundingBox() const;
    // TODO: bounding sphere
    const vector<Vec3f>& getVerts();
    const vector<Vec3f>& getNorms();
    const vector<Vec3u>& getFaces();
    const vector<Vec2f>& getUVs();

protected:
    // note: this assumes a triangle mesh
    vector<Vec3f> vs;
    vector<Vec3f> ns;
    vector<Vec2f> uvs;
    vector<Vec3u> fs;

    BBox3f bbox;
    // TODO
};
