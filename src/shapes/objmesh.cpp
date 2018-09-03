#include <feign/shapes/objmesh.h>

ObjMesh::ObjMesh() : Shape()
{
    // TODO
}

ObjMesh::ObjMesh(Node* parent) : Shape(parent)
{
    // TODO
}

string ObjMesh::getName() const
{
    return Shape::getName() + "obj";
}

uint32_t ObjMesh::tris() const
{
    throw new NotImplementedException();

    return 0;
}

uint32_t ObjMesh::verts() const
{
    throw new NotImplementedException();

    return 0;
}

float ObjMesh::surfaceArea() const
{
    throw new NotImplementedException();

    return 0.f;
}

float ObjMesh::surfaceArea(uint32_t index) const
{
    throw new NotImplementedException();

    return 0.f;
}

float ObjMesh::pdf(uint32_t index) const
{
    throw new NotImplementedException();

    return 0.f;
}

const BBox3f& ObjMesh::getBoundingBox() const { return bbox; }
const vector<Vec3f>&& ObjMesh::getVerts() const { return vs; }
const vector<Vec3f>&& ObjMesh::getNorms() const { return ns; }
const vector<Vec3u>&& ObjMesh::getFaces() const { return fs; }
const vector<Vec2f>&& ObjMesh::getUVs() const { return uvs; }
