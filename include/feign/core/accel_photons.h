/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>
#include <feign/core/sampler.h>
#include <feign/math/bbox.h>

FEIGN_BEGIN();

// TODO: make a more compact photon implementation later
struct Photon
{
    Point3f pos;
    Vector3f dir;
    Color3f power;
};

class PhotonAccel
{
public:
    PhotonAccel() { }
    virtual ~PhotonAccel() { }

    virtual void clear() = 0;
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons) = 0;
};

/////////////////////////////////////////////////
// PHOTON ARRAY (no accelerations)
/////////////////////////////////////////////////
class PhotonArray : public PhotonAccel
{
public:
    PhotonArray() { }
    virtual ~PhotonArray() { }

    virtual void clear();
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons);

    Photon* photons;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// PHOTON BVH
/////////////////////////////////////////////////
class PhotonBVH : public PhotonAccel
{
public:
    PhotonBVH() { }
    virtual ~PhotonBVH() { }

    virtual void clear();
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons);

    // TODO
    // Photon* photons;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// PHOTON KD-Tree
/////////////////////////////////////////////////
class PhotonKDTree : public PhotonAccel
{
public:
    PhotonKDTree() { }
    virtual ~PhotonKDTree() { }

    virtual void clear();
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons);

    // TODO
    // Photon* photons;
};
/////////////////////////////////////////////////

FEIGN_END();
