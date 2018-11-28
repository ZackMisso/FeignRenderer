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

bool ObjMesh::intersect(const Ray3f& ray, Intersection& its)
{
    bool intersects = false;

    for (uint32_t i = 0; i < fs.size(); ++i)
    {
        intersects |= intersect(i, ray, its);
    }

    return intersects;
}

bool ObjMesh::intersect(uint32_t face, const Ray3f& ray, Intersection& its)
{
    // Vec3u val = fs[face];
    // cout << "whoa" << endl;
    // cout << fs.size() << endl;
    // cout << vs.size() << endl;

    uint32_t i0 = fs[face][0];
    uint32_t i1 = fs[face][1];
    uint32_t i2 = fs[face][2];

    const Point3f p0 = vs[i0];
    const Point3f p1 = vs[i1];
    const Point3f p2 = vs[i2];

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;

    // WHY IS RAY DIRECTION ZERO
    // cout << "ray.dir: " << ray.dir(0) << " " << ray.dir(1) << " " << ray.dir(2) << endl;

    Vector3f pvec = ray.dir ^ edge2;

    float det = edge1 % pvec;

    if (det > -1e-8f && det < 1e-8f)
    {
        // cout << "Determinate: " << det << endl;
        // cout << "p0x: " << p0(0) << endl;
        // cout << "p0y: " << p0(1) << endl;
        // cout << "p0z: " << p0(2) << endl;
        return false;
    }

    float invDet = 1.f / det;

    Vector3f tvec = ray.origin - p0;

    its.uv[0] = (tvec % pvec) * invDet;

    if (its.uv[0] < 0.0 || its.uv[0] > 1.0)
    {
        // cout << "U" << endl;
        return false;
    }

    Vector3f qvec = tvec ^ edge1;

    its.uv[1] = (ray.dir % qvec) * invDet;

    if (its.uv[1] < 0.0 || its.uv[1] > 1.0)
    {
        // cout << "V" << endl;
        return false;
    }

    its.t = (edge2 % qvec) * invDet;

    bool ret = its.t >= ray.mint && its.t <= ray.maxt;

    // if (ret) cout << "WHOO SOMTHING INTERSECTED" << endl;

    return its.t >= ray.mint && its.t <= ray.maxt;

    // throw new NotImplementedException("obj mesh intersect");
    //
    // return false;
}

const BBox3f& ObjMesh::getBoundingBox() const { return bbox; }
const vector<Point3f>& ObjMesh::getVerts() const { return vs; }
const vector<Normal3f>& ObjMesh::getNorms() const { return ns; }
const vector<Vec3u>& ObjMesh::getFaces() const { return fs; }
const vector<Vec2f>& ObjMesh::getUVs() const { return uvs; }
