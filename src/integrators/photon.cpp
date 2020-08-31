/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

FEIGN_BEGIN()

// TODO: for now this will only support homogeneous global media

PhotonMapping::PhotonMapping(FilterNode* filter,
                             Params* params)
    : Integrator(filter, params),
      num_photons(params->num_photons) { }

void PhotonMapping::preProcess()
{
    Integrator::preProcess();
}

// TODO: maybe add a way to cache all of the photon data to a file?
// TODO: created a binary representation of all of the photon data instead of
//       ascii

Color3f PhotonMapping::scatter_photons(const Scene* scene,
                                       Sampler* sampler)
{
    // create initial list of photons
    Photon* photons = new Photon[num_photons];
    int created_photons = 0;

    // loop until we have filled our photon quota
    while (created_photons < num_photons)
    {
        // sample a light source uniformly

        // create an initial photon

        // loop for some maximum bounce count
        bool not_terminated = true;
        for (int i = 0; i < max_bounces && not_terminated; ++i)
        {
            // detect hit

            // potentially store photon in map

            // scatter

            // apply russian roulette termination
        }
    }

    return Color3f(0.f);
}

Color3f PhotonMapping::Li(const Scene* scene,
                          Sampler* sampler,
                          const Ray3f& cam_ray) const
{
    // TODO: do a direct integrator scheme

    Color3f(0.f);
}

FEIGN_END()
