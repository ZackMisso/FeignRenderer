/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <tests/ray_intersect_test.h>
#include <feign/shapes/objmesh.h>

RayIntersectTestData::RayIntersectTestData() : UnitTestData()
{
    // TODO
}

void RayIntersectTestData::logReport() const
{
    // TODO
}

RayIntersectTest::RayIntersectTest() { }

bool RayIntersectTest::evaluateTest(UnitTestData& testLog) const
{
    std::vector<Point3f> vs = std::vector<Point3f>();
    std::vector<Normal3f> ns = std::vector<Normal3f>();
    std::vector<Vec3u> fs = std::vector<Vec3u>();
    std::vector<Vec2f> uvs = std::vector<Vec2f>();
    std::vector<Triangle> tris = std::vector<Triangle>();

    vs.push_back(Point3f(1.0, 0.0, -1.0));
    vs.push_back(Point3f(1.0, 0.0, 1.0));
    vs.push_back(Point3f(-1.0, 0.0, 1.0));
    vs.push_back(Point3f(-1.0, 0.0, -1.0));

    ns.push_back(Point3f(0.0, 1.0, 0.0));

    // fs.push_back(Vec3u(1, 4, 2));
    // fs.push_back(Vec3u(4, 3, 2));

    uvs.push_back(Vec2f(0.0, 8.0));
    uvs.push_back(Vec2f(0.0, 0.0));
    uvs.push_back(Vec2f(8.0, 8.0));
    uvs.push_back(Vec2f(8.0, 0.0));

    tris.push_back(Triangle(Vec3u(0, 3, 1),
                            Vec3u(0, 0, 0),
                            Vec3u(0, 0, 0)));
    tris.push_back(Triangle(Vec3u(3, 2, 1),
                            Vec3u(0, 0, 0),
                            Vec3u(0, 0, 0)));

    Ray3f ray = Ray3f(Point3f(0.25, 5.0, 0.25), Vector3f(0.0, -1.0, 0.0));
    ObjMesh mesh = ObjMesh(vs, ns, uvs, tris);

    Intersection its;

    if (mesh.intersect(ray, its))
    {
        return true;
        // std::cout << "success" << std::endl;
    }
    else
    {
        std::cout << "fail" << std::endl;
        return false;
    }

    return true;
}
