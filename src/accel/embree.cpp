#include <feign/accel/embree.h>

void EmbreeAccel::preProcess()
{
    rtcore = "start_threads=1,set_affinity=1";
    device = rtcNewDevice(rtcore.c_str());
    scene = rtcNewScene(device);
    rtcSetDeviceErrorFunction(device, embree_error_handler, nullptr);
}

void EmbreeAccel::clear()
{
    rtcReleaseScene(scene);
    scene = nullptr;
}

void EmbreeAccel::addShape(Shape* mesh)
{
    mesh->addShapeToScene(scene, device);
    meshes.push_back(mesh);
}

void EmbreeAccel::build()
{
    rtcCommitScene(scene);
}

bool EmbreeAccel::intersect(const Ray3f& scene_ray, Intersection& its) const
{
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    /* initialize ray */
    RTCRay new_ray = EmbreeUtil::convert(scene_ray);
    RTCRayHit hit;
    hit.ray = new_ray;
    hit.hit = RTCHit();
    hit.hit.primID = RTC_INVALID_GEOMETRY_ID;
    hit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    /* intersect ray with scene */
    rtcIntersect1(scene, &context, &hit);

    if (hit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
    {
        // need to set uv, t, primId, and far
        its.t = hit.ray.tfar;
        its.uv = Vec2f(hit.hit.u, hit.hit.v);
        its.f = hit.hit.primID;

        Ray3f ray(scene_ray);
        ray.far = its.t;

        // TODO: this is inefficient
        for (int i = 0; i < meshes.size(); i++)
        {
            if (meshes[i]->getGeomID() == hit.hit.geomID)
            {
                its.intersectedMesh = meshes[i];
            }
        }

        its.intersectedMesh->completeIntersectionInfo(ray, its);

        return true;
    }

    return false;
}
