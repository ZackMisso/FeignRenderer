/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/shapes/objmesh.h>
#include <fstream>

ObjMesh::ObjMesh() : Shape()
{
    vs = std::vector<Point3f>();
    ns = std::vector<Normal3f>();
    uvs = std::vector<Vec2f>();
    tris = std::vector<Triangle>();
}

ObjMesh::ObjMesh(const std::string& filename, bool flip_norms)
    : Shape(), filename(filename), flip_norms(flip_norms)
{
    vs = std::vector<Point3f>();
    ns = std::vector<Normal3f>();
    uvs = std::vector<Vec2f>();
    tris = std::vector<Triangle>();
}

ObjMesh::ObjMesh(const std::vector<Point3f>& vs,
                 const std::vector<Normal3f>& ns,
                 const std::vector<Vec2f>& uvs,
                 const std::vector<Triangle>& tris)
    : Shape(), vs(vs), uvs(uvs), ns(ns), tris(tris)
{ // does nothing
}

uint32_t ObjMesh::num_tris() const
{
    return tris.size();
}

uint32_t ObjMesh::num_verts() const
{
    return vs.size();
}

float ObjMesh::surfaceArea() const
{
    return sa;
}

Float ObjMesh::surface_area(int index) const
{
    uint32_t i0 = tris[index].vsInds(0);
    uint32_t i1 = tris[index].vsInds(1);
    uint32_t i2 = tris[index].vsInds(2);

    Point3f p0 = vs[i0];
    Point3f p1 = vs[i1];
    Point3f p2 = vs[i2];

    return Vector3f((p1 - p0) ^ (p2 - p0)).norm() * 0.5;
}

float ObjMesh::pdf(uint32_t index) const
{
    throw new NotImplementedException("objmesh pdf index");

    return 0.f;
}

void ObjMesh::addShapeToScene(RTCScene scene, RTCDevice device)
{
    // TODO: modify this to store vertices as e_Vertex objects to save
    //       computation.
    // TODO: modify this to store triangles as e_Triangle objects
    RTCGeometry e_mesh = rtcNewGeometry (device, RTC_GEOMETRY_TYPE_TRIANGLE);

    e_Vertex* vertices = (e_Vertex*) rtcSetNewGeometryBuffer(e_mesh,
                                                             RTC_BUFFER_TYPE_VERTEX,
                                                             0,
                                                             RTC_FORMAT_FLOAT3,
                                                             sizeof(e_Vertex),
                                                             vs.size());

    for (int i = 0; i < vs.size(); ++i)
    {
        vertices[i].x = vs[i](0);
        vertices[i].y = vs[i](1);
        vertices[i].z = vs[i](2);
    }

    e_Triangle* triangles = (e_Triangle*) rtcSetNewGeometryBuffer(e_mesh,
                                                                  RTC_BUFFER_TYPE_INDEX,
                                                                  0,
                                                                  RTC_FORMAT_UINT3,
                                                                  sizeof(e_Triangle),
                                                                  tris.size());

    for (int i = 0; i < tris.size(); ++i)
    {
        triangles[i].v0 = tris[i].vsInds(0);
        triangles[i].v1 = tris[i].vsInds(1);
        triangles[i].v2 = tris[i].vsInds(2);
    }

    rtcCommitGeometry(e_mesh);
    geomID = rtcAttachGeometry(scene, e_mesh);
    rtcReleaseGeometry(e_mesh);
}

// TODO: implement a better parser
void ObjMesh::parseFromFile(const std::string& filename)
{
    if (filename.empty())
        return;

    // clear current data
    vs.clear();
    ns.clear();
    uvs.clear();
    tris.clear();

    // TODO: is there a better way of writing this so that it runs
    //       faster?
    std::ifstream ifs (filename.c_str());

    if (ifs.fail())
    {
        throw new FeignRendererException(filename);
    }

    std::string lineStr;
    while (getline(ifs, lineStr))
    {
        std::istringstream line(lineStr);

        std::string token;
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

            if (flip_norms) n = -n;

            ns.push_back(n.normalized());
        }
        else if (token == "f")
        {
            std::string f1;
            std::string f2;
            std::string f3;
            std::string f4;
            std::string f5;

            line >> f1;
            line >> f2;
            line >> f3;
            line >> f4;
            line >> f5;

            if (!f5.empty())
            {
                throw new FeignRendererException("Meshes with more than 4 verts in a face are not supported");
            }

            if (!f4.empty())
            {
                std::cout << f1 << " " << f2 << " " << f3 << " " << f4 << std::endl;
                throw new NotImplementedException("quads in obj");
            }

            Vec3u p;
            Vec3u uv;
            Vec3u n;

            std::istringstream f1_stream(f1);
            std::istringstream f2_stream(f2);
            std::istringstream f3_stream(f3);

            std::string token;

            std::getline(f1_stream, token, '/');
            p[0] = stoi(token) - 1;
            std::getline(f1_stream, token, '/');
            uv[0] = stoi(token) - 1;
            std::getline(f1_stream, token, '/');
            n[0] = stoi(token) - 1;

            std::getline(f2_stream, token, '/');
            p[1] = stoi(token) - 1;
            std::getline(f2_stream, token, '/');
            uv[1] = stoi(token) - 1;
            std::getline(f2_stream, token, '/');
            n[1] = stoi(token) - 1;

            std::getline(f3_stream, token, '/');
            p[2] = stoi(token) - 1;
            std::getline(f3_stream, token, '/');
            uv[2] = stoi(token) - 1;
            std::getline(f3_stream, token, '/');
            n[2] = stoi(token) - 1;

            tris.push_back(Triangle(p, n, uv));
        }
    }
}

void ObjMesh::infoDump()
{
    std::cout << "Number of Verts: " << vs.size() << std::endl;
    std::cout << "Number of Triangles: " << tris.size() << std::endl;

    bbox.infoDump();

    LOG("post dump");
}

uint32_t ObjMesh::primitiveCount() const
{
    return tris.size();
}

BBox3f ObjMesh::boundingBox() const
{
    return bbox;
}

BBox3f ObjMesh::boundingBox(uint32_t tri) const
{
    uint32_t i0 = tris[tri].vsInds(0);
    uint32_t i1 = tris[tri].vsInds(1);
    uint32_t i2 = tris[tri].vsInds(2);

    Point3f p0 = vs[i0];
    Point3f p1 = vs[i1];
    Point3f p2 = vs[i2];

    BBox3f box(p0, p0);
    box.expand(p1);
    box.expand(p2);
    return box;
}

Point3f ObjMesh::compute_centroid() const
{
    Point3f point = Point3f(0.0);

    for (int i = 0; i < vs.size(); ++i)
    {
        point += vs[i];
    }
    point /= double(vs.size());

    return point;
}

Point3f ObjMesh::centroid() const
{
    LOG("inside centroid");
    return center;
}

Point3f ObjMesh::centroid(uint32_t tri) const
{
    uint32_t i0 = tris[tri].vsInds(0);
    uint32_t i1 = tris[tri].vsInds(1);
    uint32_t i2 = tris[tri].vsInds(2);

    Point3f p0 = vs[i0];
    Point3f p1 = vs[i1];
    Point3f p2 = vs[i2];

    return (p0 + p1 + p2) * (1.0 / 3.0);
}

void ObjMesh::preProcess(bool requires_processing)
{
    // TODO: make these tasks multithreaded ???
    parseFromFile(filename);

    // apply the geometry shader to the mesh
    if (geomShader->shader && geomShader->shader->isValid(OBJ_MESH))
    {
        geomShader->shader->evaluate((void*)this);
    }

    center = compute_centroid();

    if (requires_processing && vs.size() > 0)
    {
        // TODO: is this really ever needed?
        // // precompute the total surface area and cache it
        // sa = 0.0;
        // for (uint32_t i = 0; i < tris.size(); ++i)
        // {
        //     sa += surface_area(i);
        // }
        //
        //

        bbox = BBox3f(vs[0], vs[0]);
        for (uint32_t i = 1; i < vs.size(); ++i)
        {
            bbox.expand(vs[i]);
        }
    }
}

bool ObjMesh::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    Ray3f ray(scene_ray);

    bool intersects = false;

    for (uint32_t i = 0; i < tris.size(); ++i)
    {
        Intersection tmp;

        if (intersect(i, ray, tmp))
        {
            intersects = true;
            its.uv = tmp.uv;
            its.t = tmp.t;
            its.f = i;
            ray.far = its.t;
        }
    }

    return intersects;
}

// I do not think I need this function anymore, but I am going to leave it commented
// out here for reference if need be later
bool ObjMesh::intersect(uint32_t face, const Ray3f& ray, Intersection& its) const
{
    uint32_t i0 = tris[face].vsInds(0);
    uint32_t i1 = tris[face].vsInds(1);
    uint32_t i2 = tris[face].vsInds(2);

    const Point3f p0 = vs[i0];
    const Point3f p1 = vs[i1];
    const Point3f p2 = vs[i2];

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;

    Vector3f pvec = ray.dir ^ edge2;

    float det = edge1 % pvec;

    if (det > -1e-8f && det < 1e-8f)
    {
        return false;
    }

    float invDet = 1.f / det;

    Vector3f tvec = ray.origin - p0;

    its.uv[0] = (tvec % pvec) * invDet;

    if (its.uv[0] < 0.0 || its.uv[0] > 1.0)
    {
        return false;
    }

    Vector3f qvec = tvec ^ edge1;

    its.uv[1] = (ray.dir % qvec) * invDet;

    if (its.uv[1] < 0.0 || its.uv[1] > 1.0)
    {
        return false;
    }

    its.t = (edge2 % qvec) * invDet;

    return its.t >= ray.near && its.t <= ray.far;
}

void ObjMesh::completeIntersectionInfo(Intersection& its) const
{
    Vec3f bary(1.0 - its.uv(0) - its.uv(1), its.uv(0), its.uv(1));

    its.bary = Point3f(bary);

    uint32_t i0_vs = tris[its.f].vsInds(0);
    uint32_t i1_vs = tris[its.f].vsInds(1);
    uint32_t i2_vs = tris[its.f].vsInds(2);

    Point3f p0 = vs[i0_vs];
    Point3f p1 = vs[i1_vs];
    Point3f p2 = vs[i2_vs];

    // compute intersection position
    its.p = p0 * bary(0) + p1 * bary(1) + p2 * bary(2);

    // compute mesh texture coordinates
    if (uvs.size() > 0)
    {
        uint32_t i0_uvs = tris[its.f].uvsInds(0);
        uint32_t i1_uvs = tris[its.f].uvsInds(1);
        uint32_t i2_uvs = tris[its.f].uvsInds(2);

        its.uv = uvs[i0_uvs] * bary(0) + uvs[i1_uvs] * bary(1) + uvs[i2_uvs] * bary(2);
    }

    // compute the geometric frame
    its.g_frame = CoordinateFrame( ((p1-p0)^(p2-p0)).normalized() );

    // compute the normals
    if (ns.size() > 0)
    {
        uint32_t i0_ns = tris[its.f].nsInds(0);
        uint32_t i1_ns = tris[its.f].nsInds(1);
        uint32_t i2_ns = tris[its.f].nsInds(2);

        its.s_frame = CoordinateFrame((ns[i0_ns] * bary(0) +
                                       ns[i1_ns] * bary(1) +
                                       ns[i2_ns] * bary(2)).normalized());
    }
    else
    {
        its.s_frame = its.g_frame;
    }
}

const std::vector<Triangle>& ObjMesh::getTris() const { return tris; }
const std::vector<Point3f>& ObjMesh::getVerts() const { return vs; }
const std::vector<Normal3f>& ObjMesh::getNorms() const { return ns; }
const std::vector<Vec2f>& ObjMesh::getUVs() const { return uvs; }

void ObjMesh::setVerts(std::vector<Point3f>& param) { vs = param; }
void ObjMesh::setNorms(std::vector<Normal3f>& param) { ns = param; }
void ObjMesh::setUVs(std::vector<Vec2f>& param) { uvs = param; }
