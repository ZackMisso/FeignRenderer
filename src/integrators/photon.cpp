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

// TODO: focus on getting this working first for a base scene, then work on
//       converting this to work with the shader framework

// TODO: fixed the entire shading framework, it should not have as many layers
//       of abstraction as it does

// TODO: for now this will only support homogeneous global media

PhotonMapping::PhotonMapping(FilterNode* filter,
                             Params* params)
    : Integrator(filter, params),
      num_photons(params->num_photons) { }

// Integrator pre-processing now has to happen last
void PhotonMapping::preProcess(const Scene* scene,
                               Sampler* sampler)
{
    Integrator::preProcess(scene, sampler);

    scatter_photons(scene, sampler);
}

// TODO: maybe add a way to cache all of the photon data to a file?
// TODO: created a binary representation of all of the photon data instead of
//       ascii

Color3f PhotonMapping::scatter_photons(const Scene* scene,
                                       Sampler* sampler)
{
    // create initial list of photons
    Photon* photons = new Photon[num_photons]();
    int created_photons = 0;

    // loop until we have filled our photon quota
    while (created_photons < num_photons)
    {
        // predefine this so it does not have to get recreated every loop
        MaterialClosure closure = MaterialClosure(sampler,
                                                  scene,
                                                  false,
                                                  true);

        // sample a light source uniformly
        int index = int(sampler->next1D() * float(scene->emitters.size()));
        float emitter_pdf = 1.f / float(scene->emitters.size());
        Emitter* emitter = scene->emitters[index];

        // sample the initial location and direction
        EmitterQuery eqr;
        Float query_pdf = 0.f;
        Color3f power = emitter->sample_li(eqr, sampler->next2D(), &query_pdf);
        // Color3f power = Color3f(0.f); // TODO: fix all this
        emitter_pdf *= query_pdf;

        // create an initial photon
        Photon current_photon = Photon(eqr.p, eqr.wi, power);

        // create the initial ray
        Ray3f ray = Ray3f(current_photon.pos,
                          current_photon.dir,
                          Epsilon,
                          10000000.0); // TODO: replace with actual floating max

        // loop for some maximum bounce count
        bool not_terminated = true;
        for (int i = 0; i < max_bounces && not_terminated; ++i)
        {
            // detect hit
            Intersection its;

            if (!scene->intersect_non_null(ray, its))
            {
                break;
            }

            // evaluate shader / colliding location
            const MaterialShader* shader = scene->getShapeMaterialShader(its);

            // TODO: get closure working for this case
            // closure.its = &its;
            // closure.ray = &ray;
            // closure.wi = its.toLocal(-ray.dir);
            // closure.emission = COLOR_BLACK;
            // closure.nee = COLOR_BLACK;
            // closure.albedo = COLOR_BLACK;
            //
            // // evaluate the material shader
            // shader->evaluate(closure);
            shader->evaluate_for_photon(closure);

            // potentially store photon in map
            if (!closure.is_specular)
            {
                // decide whether or not to store photon
            }

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

// void PhotonMapping::render(const Scene* scene,
//                            const Camera* camera,
//                            Sampler* sampler,
//                            Imagef& image) const
// {
//     // scatter_photons(scene, sampler);
//
//     Integrator::render(scene,
//                        camera,
//                        sampler,
//                        image);
// }
//
// void PhotonMapping::render_fast(const Scene* scene,
//                                 const Camera* camera,
//                                 Sampler* sampler,
//                                 Imagef& image) const
// {
//     // scatter_photons(scene, sampler);
//
//     Integrator::render(scene,
//                        camera,
//                        sampler,
//                        image);
// }

FEIGN_END()
