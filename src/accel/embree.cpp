/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel.h>

EmbreeAccel::~EmbreeAccel()
{
    clear();
}

void EmbreeAccel::preProcess()
{
    // preProcessChisldren();

    // initialize embree datastructures
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

// adds all intersectable meshes in a scene to embree's backend
void EmbreeAccel::addShape(Shape* mesh)
{
    LOG("adding shape to scene");
    mesh->addShapeToScene(scene, device);
    LOG("pushing back mesh");
    meshes.push_back(mesh);
    LOG("finished adding shape");
}

// commits all changes for this scene. This must be called after all scene
// geometry has been defined but before rendering
void EmbreeAccel::build()
{
    rtcCommitScene(scene);
}

// the interface between the integrators and embree
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
        // need to set uv, t, primId, and update far
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
                its.intersected_mesh = meshes[i];
            }
        }

        // complete the intersection information by calculating normals / uv's /
        // etc.
        // TODO: this is redundant and should not be used since embree already
        // handles this information for us
        its.intersected_mesh->completeIntersectionInfo(ray, its);

        return true;
    }

    return false;
}
