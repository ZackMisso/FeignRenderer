/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel_ray.h>
#include <feign/stats/clocker.h>

FEIGN_BEGIN()

EmbreeAccel::EmbreeAccel()
{
    meshes = std::vector<Shape *>();
}

EmbreeAccel::~EmbreeAccel()
{
    clear();
}

void EmbreeAccel::preProcess()
{
    // initialize embree datastructures
    // TODO: get back up to date with figuring out what parameters I
    //       should feed into embree
    rtcore = "start_threads=1";
    device = rtcNewDevice(rtcore.c_str());
    scene = rtcNewScene(device);
    rtcSetDeviceErrorFunction(device, embree_error_handler, nullptr);
}

void EmbreeAccel::clear()
{
    rtcReleaseScene(scene);
    scene = nullptr;
}

// adds all intersectable meshes in a scene to embree's backend
void EmbreeAccel::addShape(Shape *mesh)
{
    mesh->addShapeToScene(scene, device);
    meshes.push_back(mesh);
}

// commits all changes for this scene. This must be called after all scene
// geometry has been defined but before rendering
void EmbreeAccel::build()
{
    rtcCommitScene(scene);
}

// the interface between the integrators and embree
bool EmbreeAccel::intersect(const Ray3f &scene_ray, Intersection &its) const
{
    CLOCKER_START_ONE(ClockerType::EMBREE)

    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    /* initialize ray */
    RTCRay new_ray = convert_ray_to_rtcray(scene_ray);
    RTCRayHit hit;
    hit.ray = new_ray;
    hit.hit = RTCHit();
    hit.hit.primID = RTC_INVALID_GEOMETRY_ID;
    hit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    /* intersect ray with scene */
    rtcIntersect1(scene, &context, &hit);

    if (hit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
    {
        // need to set uv, t, primId, and update far
        its.t = hit.ray.tfar;
        its.uv = Vec2f(hit.hit.u, hit.hit.v);
        its.f = hit.hit.primID;

        // Ray3f ray(scene_ray);
        // ray.far = its.t;

        // TODO: note in the future the geom id might be different from the index
        //       if instancing is implemented in the future

        its.intersected_mesh = meshes[hit.hit.geomID];

        // complete the intersection information by calculating smooth normals /
        // uv's // etc.
        its.intersected_mesh->completeIntersectionInfo(its);

        CLOCKER_STOP_ONE(ClockerType::EMBREE)

        return true;
    }

    CLOCKER_STOP_ONE(ClockerType::EMBREE)

    return false;
}

FEIGN_END()
