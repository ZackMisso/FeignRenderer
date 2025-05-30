/**
 * Author:    Zackary Misso
 * Version:   0.1.0
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

// TODO: i don't remember the state of the quality of these light
//       sampling techniques. I need to retest if this code works.

FEIGN_BEGIN()

void PhotonArray::build(const BBox3f &scene_bounds,
                        Photon *list,
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
        Float dist = (pt - photons[i].pos).sqrNorm();
        if (dist < radius)
            return true;
    }
    return false;
}

void PhotonArray::eval(MaterialClosure &closure,
                       const MaterialShader *shader,
                       const Point3f &pt,
                       Float radius) const
{
    Float sqr_radius = radius;
    Float inv_area = 1.f / (M_PI * sqr_radius);
    for (int i = 0; i < num_photons; ++i)
    {
        if ((pt - photons[i].pos).sqrNorm() < sqr_radius)
        {
            Color3f pwr_div_area = photons[i].power * inv_area;

            // evaluate the material
            closure.wo = closure.its->toLocal(photons[i].dir);
            // closure.wi = photons[i].dir;
            closure.albedo = COLOR_BLACK;
            shader->evaluate_mat_only(closure);

            // results are stored in closure.nee since photon mapping does not utilize nee
            closure.nee += closure.albedo * pwr_div_area;
        }
    }
}

void PhotonArray::maybeAddPhoton(std::vector<std::pair<Float, int>> &closest_k,
                                 const Point3f &pt,
                                 int k,
                                 int photon) const
{
    Float dist = (pt - photons[photon].pos).sqrNorm();

    if (!closest_k.size())
    {
        closest_k.push_back(std::pair<Float, int>(dist, photon));
        return;
    }

    int index = photon;

    for (int i = 0; i < closest_k.size(); ++i)
    {
        if (dist < closest_k[i].first)
        {
            std::pair<Float, int> new_pair = std::pair<Float, int>(dist, index);
            dist = closest_k[i].first;
            index = closest_k[i].second;
            closest_k[i] = new_pair;
        }
    }

    if (closest_k.size() < k)
    {
        closest_k.push_back(std::pair<Float, int>(dist, index));
    }
}

// this is not the fastest way of handling this, but i don't care to be honest,
// this datastructure should not be used much longer anyways

void PhotonArray::eval(MaterialClosure &closure,
                       const MaterialShader *shader,
                       const Point3f &pt,
                       int k_photons) const
{
    std::vector<std::pair<Float, int>> closest_k = std::vector<std::pair<Float, int>>();

    for (int i = 0; i < num_photons; ++i)
    {
        maybeAddPhoton(closest_k, pt, k_photons, i);
    }

    Float inv_area = 1.f / (M_PI * closest_k[k_photons - 1].first);

    for (int i = 0; i < closest_k.size() - 1; ++i)
    {
        // debug logic
        if (closest_k[i].first > closest_k[k_photons - 1].first)
            assert(false);

        int index = closest_k[i].second;

        Color3f pwr_div_area = photons[index].power * inv_area;

        // evaluate the material
        closure.wo = closure.its->toLocal(photons[index].dir);
        // if (std::abs(closure.wo.sqrNorm() - 1.0) > Epsilon)
        // {
        //     LOG("norm: " + STR(closure.wo.sqrNorm()));
        //     assert(false);
        // }
        // closure.wi = photons[i].dir;

        closure.albedo = COLOR_BLACK;
        shader->evaluate_mat_only(closure);

        // results are stored in closure.nee since photon mapping does not utilize nee
        closure.nee += closure.albedo * pwr_div_area;
    }
}

void PhotonArray::eval(MaterialClosure &closure,
                       const MaterialShader *shader,
                       const Point3f &pt,
                       Float rad_1,
                       Float rad_2) const
{
    throw new NotImplementedException("photon array");
}

FEIGN_END()
