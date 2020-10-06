/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

// this class implementation is just a simple list to be used as a placeholder
// for more efficient and complex acceleration structures. This should be used
// as a means to test correctness of those implementations

 #include <feign/core/accel_photons.h>

 void PhotonArray::build(const BBox3f& scene_bounds,
                         Photon* list,
                         int count)
 {
     photons = list;
     num_photons = count;
 }

 void PhotonArray::clear()
 {
     delete[] photons;
 }

 bool PhotonArray::nearPhoton(Point3f pt, Float radius) const
 {
     Float square_radius = radius * radius;
     for (int i = 0; i < num_photons; ++i)
     {
         if ( (pt - photons[i].pos).sqrNorm() < radius ) return true;
     }
     return false;
 }
