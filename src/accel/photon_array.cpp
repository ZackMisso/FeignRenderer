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
#include <feign/core/closure.h>
#include <feign/core/shader.h>

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

void PhotonArray::eval(MaterialClosure& closure,
                       const MaterialShader* shader,
                       const Point3f& pt,
                       Float radius) const
{
    Float sqr_radius = radius * radius;
    Float inv_area = 1.f / (M_PI * sqr_radius);
    for (int i = 0; i < num_photons; ++i)
    {
        if ( (pt - photons[i].pos).norm() < sqr_radius )
        {
            Color3f pwr_div_area = photons[i].power * inv_area;

            // evaluate the material
            // closure.wi = closure.its->toLocal(photons[i].dir);
            closure.wi = photons[i].dir;
            closure.albedo = COLOR_BLACK;
            shader->evaluate_mat_only(closure);

            // results are stored in closure.nee since photon mapping does not utilize nee
            closure.nee += closure.albedo * pwr_div_area;
        }
    }
}


void PhotonArray::eval(MaterialClosure& closure,
                       const MaterialShader* shader,
                       const Point3f& pt,
                       int k_photons) const
{
    throw new NotImplementedException("photon array");
}

void PhotonArray::eval(MaterialClosure& closure,
                       const MaterialShader* shader,
                       const Point3f& pt,
                       Float rad_1,
                       Float rad_2) const
{
    throw new NotImplementedException("photon array");
}
