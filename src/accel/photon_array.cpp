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
    // LOG("photons: " + STR(num_photons));
    for (int i = 0; i < num_photons; ++i)
    {
        Float dist = (pt - photons[i].pos).sqrNorm();
        // LOG("dist: " + STR(dist));
        if ( (pt - photons[i].pos).sqrNorm() < radius ) return true;
    }
    return false;
}

void PhotonArray::eval(const Point3f& pt,
                       Float radius,
                       std::vector<Color3f>& pwr_div_area) const
{
    Float sqr_radius = radius * radius;
    Float inv_area = 1.f / (M_PI * sqr_radius);
    for (int i = 0; i < num_photons; ++i)
    {
        if ( (pt - photons[i].pos).norm() < sqr_radius )
        {
            pwr_div_area.push_back(photons[i].power * inv_area);
        }
    }
}


void PhotonArray::eval(const Point3f& pt,
                       int k_photons,
                       std::vector<Color3f>& pwr_div_area) const
{
    throw new NotImplementedException("photon array");
}

void PhotonArray::eval(const Point3f& pt,
                       Float rad_1,
                       Float rad_2,
                       std::vector<Color3f>& pwr_div_area) const
{
    throw new NotImplementedException("photon array");
}
