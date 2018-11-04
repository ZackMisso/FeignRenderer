// This code is based off of Wenzel Jakob's Nori ray tracer

#include <feign/shapes/objmesh.h>
#include <fstream>

ObjMesh::ObjMesh() : Shape()
{
    vs = vector<Point3f>();
    ns = vector<Normal3f>();
    uvs = vector<Vec2f>();
    fs = vector<Vec3u>();
}

ObjMesh::ObjMesh(Node* parent) : Shape(parent)
{
    vs = vector<Point3f>();
    ns = vector<Normal3f>();
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
    throw new NotImplementedException("objmesh surface area");

    return 0.f;
}

float ObjMesh::surfaceArea(uint32_t index) const
{
    throw new NotImplementedException("objmesh surface area index");

    return 0.f;
}

float ObjMesh::pdf(uint32_t index) const
{
    throw new NotImplementedException("objmesh pdf index");

    return 0.f;
}

void ObjMesh::parseFromFile(const string& filename, Transform transform, bool flipNorms)
{
    // clear current data
    vs.clear();
    ns.clear();
    uvs.clear();
    fs.clear();

    ifstream ifs (filename.c_str());

    if (ifs.fail())
    {
        throw new OBJUnopenableException(filename);
    }

    string lineStr;
    while (getline(ifs, lineStr))
    {
        std::istringstream line(lineStr);

        string token;
        line >> token;

        if (token == "v")
        {
            Point3f p;
            line >> p[0];
            line >> p[1];
            line >> p[2];
            p = transform * p;
            vs.push_back(p);
        }
        else if (token == "vt")
        {
            Vec2f p;
            line >> p[0];
            line >> p[1];
            uvs.push_back(p);
        }
        else if (token == "vn")
        {
            Normal3f n;
            line >> n[0];
            line >> n[1];
            line >> n[2];
            if (flipNorms) n = -n;
            n = transform * n;
            n = n.normalized();
            ns.push_back(n);
        }
        else if (token == "f")
        {
            string v1;
            string v2;
            string v3;
            string v4;

            line >> v1;
            line >> v2;
            line >> v3;
            line >> v4;

            if (!v4.empty())
            {
                throw new NotImplementedException("quads in obj");
            }

            Vec3u f;
            f[0] = stoi(v1);
            f[1] = stoi(v2);
            f[2] = stoi(v3);
            fs.push_back(f);
        }
    }
}

void ObjMesh::preProcess()
{
    preProcessChildren();

    string filename;
    primitives->findString("filename", filename, "");

    int flipNorms;
    primitives->findInt("flipNorms", flipNorms, 0);

    // TODO: future
    // In the current setup, this transform is not stored
    // in the future to save memory, this should be cached and only
    // one object instance should be stored.
    Transform toWorld;
    primitives->findTransform("toWorld", toWorld, Transform());

    if (filename.empty())
    {
        throw new MissingPrimitiveException("obj mesh filename");
    }

    parseFromFile(filename, toWorld, flipNorms);
}

const BBox3f& ObjMesh::getBoundingBox() const { return bbox; }
const vector<Point3f>& ObjMesh::getVerts() const { return vs; }
const vector<Normal3f>& ObjMesh::getNorms() const { return ns; }
const vector<Vec3u>& ObjMesh::getFaces() const { return fs; }
const vector<Vec2f>& ObjMesh::getUVs() const { return uvs; }
