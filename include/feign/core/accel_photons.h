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

class MaterialClosure;
class MaterialShader;

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

// TODO: move theses notes to the photon acceleration structure files
// TODO: maybe add a way to cache all of the photon data to a file?
// TODO: created a binary representation of all of the photon data instead of
//       ascii
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
    virtual bool nearPhoton(Point3f pt,
                            Float radius) const { return false; }

    // evaluate all photons in a given radius
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      Float radius) const = 0;
    // evaluate nearest k photons
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      int k_photons) const = 0;
    // evaluate kernel
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      Float rad_1,
                      Float rad_2) const = 0;
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

    // utility methods
    void maybeAddPhoton(std::vector<std::pair<Float, int> >& closest_k,
                        const Point3f& pt,
                        int k,
                        int photon) const;

    // evaluate all photons in a given radius
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      Float radius) const;
    // evaluate nearest k photons
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      int k_photons) const;
    // evaluate kernel
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      Float rad_1,
                      Float rad_2) const;

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
    struct BVHNode
    {
        BVHNode();
        BVHNode(BVHNode* parent);
        BVHNode(const BBox3f& bounds,
                const std::vector<Photon*>& photons);
        BVHNode(BVHNode* parent,
                const BBox3f& bounds,
                const std::vector<Photon*>& photons);
        ~BVHNode();

        // this is the main build routine
        void split(Float radius, int k);

        BVHNode* traverse(Point3f point);

        void getAllPhotonsInRadius(std::vector<Photon*>& photons,
                                   Point3f point,
                                   Float radius) const;
        void getClosestKPhotons(std::vector<Photon*>& photons,
                                Point3f point,
                                int k) const;

        BBox3f bounds;
        std::vector<Photon*> photons;

        BVHNode* parent;
        BVHNode* x1_y1_z1;
        BVHNode* x1_y2_z1;
        BVHNode* x1_y1_z2;
        BVHNode* x1_y2_z2;
        BVHNode* x2_y1_z1;
        BVHNode* x2_y2_z1;
        BVHNode* x2_y1_z2;
        BVHNode* x2_y2_z2;
    };

    PhotonBVH() { }
    virtual ~PhotonBVH() { }

    virtual void clear();
    virtual void build(const BBox3f& scene_bounds,
                       Photon* photons,
                       int count);

    // used for debugging
    virtual bool nearPhoton(Point3f pt, Float radius) const;

    // evaluate all photons in a given radius
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      Float radius) const;
    // evaluate nearest k photons
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      int k_photons) const;
    // evaluate kernel
    virtual void eval(MaterialClosure& closure,
                      const MaterialShader* shader,
                      const Point3f& pt,
                      Float rad_1,
                      Float rad_2) const;

    BVHNode* photon_bvh;
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
