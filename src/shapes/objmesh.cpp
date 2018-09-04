#include <feign/shapes/objmesh.h>

ObjMesh::ObjMesh() : Shape()
{
    vs = vector<Vec3f>();
    ns = vector<Vec3f>();
    uvs = vector<Vec2f>();
    fs = vector<Vec3u>();
}

ObjMesh::ObjMesh(Node* parent) : Shape(parent)
{
    vs = vector<Vec3f>();
    ns = vector<Vec3f>();
    uvs = vector<Vec2f>();
    fs = vector<Vec3u>();
}

string ObjMesh::getName() const
{
    return Shape::getName() + "obj";
}

uint32_t ObjMesh::tris() const
{
    return fs.size();
}

uint32_t ObjMesh::verts() const
{
    return vs.size();
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

void ObjMesh::parseFromFile(const string& filename)
{
    // clear current data
    vs.clear();
    ns.clear();
    uvs.clear();
    fs.clear();

    throw new NotImplementedException();

    ifstream ifs (filename);

    if (ifs.fail())
    {
        throw new OBJUnopenableException(filename);
    }

    // TODO
}

const BBox3f& ObjMesh::getBoundingBox() const { return bbox; }
const vector<Vec3f>& ObjMesh::getVerts() const { return vs; }
const vector<Vec3f>& ObjMesh::getNorms() const { return ns; }
const vector<Vec3u>& ObjMesh::getFaces() const { return fs; }
const vector<Vec2f>& ObjMesh::getUVs() const { return uvs; }
