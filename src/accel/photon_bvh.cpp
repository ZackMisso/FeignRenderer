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
    bounds = BBox3f();
    num_photons = 0;
    photons = nullptr;
    parent = nullptr;
    x1_y1_z1 = nullptr;
    x1_y2_z1 = nullptr;
    x1_y1_z2 = nullptr;
    x1_y2_z2 = nullptr;
    x2_y1_z1 = nullptr;
    x2_y2_z1 = nullptr;
    x2_y1_z2 = nullptr;
    x2_y2_z2 = nullptr;
}

PhotonBVH::BVHNode::BVHNode(BVHNode* parent)
    : parent(parent)
{
    bounds = BBox3f();
    num_photons = 0;
    photons = nullptr;
    x1_y1_z1 = nullptr;
    x1_y2_z1 = nullptr;
    x1_y1_z2 = nullptr;
    x1_y2_z2 = nullptr;
    x2_y1_z1 = nullptr;
    x2_y2_z1 = nullptr;
    x2_y1_z2 = nullptr;
    x2_y2_z2 = nullptr;
}

PhotonBVH::BVHNode::BVHNode(const BBox3f& bounds,
                            Photon* photons,
                            int count)
    : bounds(bounds),
      photons(photons),
      num_photons(count)
{
    parent = nullptr;
    x1_y1_z1 = nullptr;
    x1_y2_z1 = nullptr;
    x1_y1_z2 = nullptr;
    x1_y2_z2 = nullptr;
    x2_y1_z1 = nullptr;
    x2_y2_z1 = nullptr;
    x2_y1_z2 = nullptr;
    x2_y2_z2 = nullptr;
}

PhotonBVH::BVHNode::~BVHNode()
{
    if (photons) delete[] photons;
    else
    {
        delete x1_y1_z1;
        delete x1_y2_z1;
        delete x1_y1_z2;
        delete x1_y2_z2;
        delete x2_y1_z1;
        delete x2_y2_z1;
        delete x2_y1_z2;
        delete x2_y2_z2;
    }
    parent = nullptr;
}

void PhotonBVH::BVHNode::getAllPhotonsInRadius(std::vector<Photon*>& photons,
                                               Point3f point,
                                               Float radius) const
{
    // TODO
}

void PhotonBVH::BVHNode::getClosestKPhotons(std::vector<Photon*>& photons,
                                            Point3f point,
                                            int k) const
{
    // TODO
}

void PhotonBVH::BVHNode::split(Float radius, int k)
{
    // the termination condition of the split routine depends on which parameter
    // is validly assigned. If radius is non-negative then this bvh will continue
    // to split until the point where if the bvh split further, each box would be
    // smaller than 2x the radius. If k is assigned, then the bvh will split until
    // a box would no longer contain k photons. If both parameters are negative,
    // then the later strategy will be used.

    throw new NotImplementedException("photon_bvh");
}

PhotonBVH::BVHNode* traverse_util(PhotonBVH::BVHNode* to_traverse,
                                  PhotonBVH::BVHNode* current_found,
                                  Point3f point,
                                  int& found_cnt)
{
    PhotonBVH::BVHNode* tmp = to_traverse->traverse(point);
    if (tmp)
    {
        found_cnt++;
        return tmp;
    }
    return current_found;
}

// this method returns the smallest bvhnode which uniquely contains a specific
// point. If a point lies on the boundry between two bounding boxes, then the
// largest bounding box which contains the point will be returned.
PhotonBVH::BVHNode* PhotonBVH::BVHNode::traverse(Point3f point)
{
    if (bounds.contains(point))
    {
        if (photons) return this;

        PhotonBVH::BVHNode* found = nullptr;
        int found_cnt = 0;

        found = traverse_util(x1_y1_z1, found, point, found_cnt);
        found = traverse_util(x1_y2_z1, found, point, found_cnt);
        found = traverse_util(x1_y1_z2, found, point, found_cnt);
        found = traverse_util(x1_y2_z2, found, point, found_cnt);
        found = traverse_util(x2_y1_z1, found, point, found_cnt);
        found = traverse_util(x2_y2_z1, found, point, found_cnt);
        found = traverse_util(x2_y1_z2, found, point, found_cnt);
        found = traverse_util(x2_y2_z2, found, point, found_cnt);

        if (found_cnt > 1) return this;
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// BVH NODE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// PHOTON BVH
////////////////////////////////////////////////////////////////////////////////
void PhotonBVH::clear()
{
    delete photon_bvh;
}

void PhotonBVH::build(const BBox3f& scene_bounds,
                      Photon* photons,
                      int count)
{
    photon_bvh = new BVHNode(scene_bounds,
                             photons,
                             count);

    photon_bvh->split(0.1, -1);
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
