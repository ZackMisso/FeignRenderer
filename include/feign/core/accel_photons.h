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
    Photon()
        : pos(Vector3f(0.f)),
          dir(Vector3f(0.f, 1.f, 0.f)),
          power(Color3f(1.f)) { }

    Photon(Point3f pos, Vector3f dir, Color3f power)
        : pos(pos), dir(dir), power(power) { }

    Photon(const Photon& other)
        : pos(other.pos), dir(other.dir), power(other.power) { }

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
                       Photon* photons,
                       int count) = 0;

    // test functions
    virtual bool nearPhoton(Point3f pt, Float radius) const { return false; }

    // TODO: should these methods modify arrays?
    // evaluate all photons in a given radius
    virtual void eval(const Point3f& pt,
                      Float radius,
                      std::vector<Color3f>& pwr_div_area) const = 0;
    // evaluate nearest k photons
    virtual void eval(const Point3f& pt,
                      int k_photons,
                      std::vector<Color3f>& pwr_div_area) const = 0;
    // evaluate kernel
    virtual void eval(const Point3f& pt,
                      Float rad_1,
                      Float rad_2,
                      std::vector<Color3f>& pwr_div_area) const = 0;
};

/////////////////////////////////////////////////
// PHOTON ARRAY (no accelerations)
/////////////////////////////////////////////////
class PhotonArray : public PhotonAccel
{
public:
    PhotonArray() : PhotonAccel() { }
    virtual ~PhotonArray() { }

    virtual void clear();
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons,
                       int count);

    // used for debugging
    virtual bool nearPhoton(Point3f pt, Float radius) const;

    // TODO: should these methods modify arrays?
    // evaluate all photons in a given radius
    virtual void eval(const Point3f& pt,
                      Float radius,
                      std::vector<Color3f>& pwr_div_area) const;
    // evaluate nearest k photons
    virtual void eval(const Point3f& pt,
                      int k_photons,
                      std::vector<Color3f>& pwr_div_area) const;
    // evaluate kernel
    virtual void eval(const Point3f& pt,
                      Float rad_1,
                      Float rad_2,
                      std::vector<Color3f>& pwr_div_area) const;

    Photon* photons;
    int num_photons;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// PHOTON BVH
// TODO in the future
/////////////////////////////////////////////////
class PhotonBVH : public PhotonAccel
{
public:
    PhotonBVH() { }
    virtual ~PhotonBVH() { }

    virtual void clear();
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons,
                       int count);

    // TODO
    // Photon* photons;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// PHOTON KD-Tree
// TODO in the future
/////////////////////////////////////////////////
class PhotonKDTree : public PhotonAccel
{
public:
    PhotonKDTree() { }
    virtual ~PhotonKDTree() { }

    virtual void clear();
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons,
                       int count);

    // TODO
    // Photon* photons;
};
/////////////////////////////////////////////////

FEIGN_END();
