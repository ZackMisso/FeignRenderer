/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/accel_photons.h>
#include <feign/core/closure.h>
#include <feign/core/shader.h>


////////////////////////////////////////////////////////////////////////////////
/// BVH NODE
////////////////////////////////////////////////////////////////////////////////
PhotonBVH::BVHNode::BVHNode()
{
    throw new NotImplementedException("photon_bvh");
}

PhotonBVH::BVHNode::~BVHNode()
{
    throw new NotImplementedException("photon_bvh");
}

void PhotonBVH::BVHNode::split()
{
    throw new NotImplementedException("photon_bvh");
}
////////////////////////////////////////////////////////////////////////////////
/// BVH NODE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// PHOTON BVH
////////////////////////////////////////////////////////////////////////////////
void PhotonBVH::clear()
{
    throw new NotImplementedException("photon_bvh");
}

void PhotonBVH::build(const BBox3f& scene_bounds,
                      Photon* photons,
                      int count)
{
    throw new NotImplementedException("photon_bvh");
}

bool PhotonBVH::nearPhoton(Point3f pt, Float radius) const
{
    throw new NotImplementedException("photon_bvh");
    return false;
}

void PhotonBVH::eval(MaterialClosure& closure,
                     const MaterialShader* shader,
                     const Point3f& pt,
                     Float radius) const
{
    throw new NotImplementedException("photon_bvh");
}

void PhotonBVH::eval(MaterialClosure& closure,
                     const MaterialShader* shader,
                     const Point3f& pt,
                     int k_photons) const
{
    throw new NotImplementedException("photon_bvh");
}

void PhotonBVH::eval(MaterialClosure& closure,
                     const MaterialShader* shader,
                     const Point3f& pt,
                     Float rad_1,
                     Float rad_2) const
{
    throw new NotImplementedException("photon_bvh");
}
////////////////////////////////////////////////////////////////////////////////
/// PHOTON BVH
////////////////////////////////////////////////////////////////////////////////
