#include <feign/shapes/objmesh.h>
#include <fstream>

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

    Transform transform; // this is just a place holder

    throw new NotImplementedException();

    ifstream ifs (filename);

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

    preProcess();
}

void ObjMesh::preProcess()
{
    throw new NotImplementedException("obj mesh pre process");
}

const BBox3f& ObjMesh::getBoundingBox() const { return bbox; }
const vector<Point3f>& ObjMesh::getVerts() const { return vs; }
const vector<Normal3f>& ObjMesh::getNorms() const { return ns; }
const vector<Vec3u>& ObjMesh::getFaces() const { return fs; }
const vector<Vec2f>& ObjMesh::getUVs() const { return uvs; }
