/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>
#include <feign/stats/clocker.h>

FEIGN_BEGIN()

// TODO: focus on getting this working first for a base scene, then work on
//       converting this to work with the shader framework

// TODO: make a volumetric version of photon mapping
// TODO: make a progressive version of photon mapping

PhotonMapping::PhotonMapping(FilterNode *filter,
                             Params *params)
    : Integrator(filter, params),
      num_photons(params->num_photons) {}

PhotonMapping::~PhotonMapping()
{
    delete photon_storage;
}

// Integrator pre-processing now has to happen last
void PhotonMapping::preProcess(const Scene *scene,
                               Sampler *sampler)
{
    // initialize clocker instances
#if CLOCKING
    Clocker::addClocker("scatter photons");
    Clocker::addClocker("eval photons");
    Clocker::addClocker("build photon accel");
#endif

    Integrator::preProcess(scene, sampler);

    photon_storage = new PhotonArray();
    scatter_photons(scene, sampler);
}

// TODO: this needs to be parallelized
void PhotonMapping::scatter_photons(const Scene *scene,
                                    Sampler *sampler)
{
#if CLOCKING
    Clocker::startClock("scatter photons");
#endif

    // create initial list of photons
    Photon *photons = new Photon[num_photons]();
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
        Float emitter_pdf = 1.f;
        Emitter *emitter = scene->choose_emitter(sampler, &emitter_pdf);

        // sample the initial location and direction
        EmitterQuery eqr;
        Float query_pdf = 1.f;
        Color3f power = emitter->sample_ray(eqr,
                                            sampler->next2D(),
                                            sampler->next2D(),
                                            &query_pdf);
        emitter_pdf *= query_pdf;
        power /= emitter_pdf;

        // create the initial ray
        Ray3f ray = Ray3f(eqr.p,
                          eqr.wi,
                          Epsilon,
                          10000000.0); // TODO: replace with actual floating max

        // loop for some maximum bounce count
        bool not_terminated = true;
        for (int i = 0; i < max_bounces && not_terminated; ++i)
        {
            // detect hit
            Intersection its;
            Point3f old_p = ray.origin;

            if (!scene->intersect_non_null(ray, its))
            {
                break;
            }

            // evaluate shader / colliding location
            const MaterialShader *shader = scene->getShapeMaterialShader(its);
            closure.albedo = 0.0;
            closure.pdf = 1.0;

            shader->evaluate_for_photon(closure);

            // potentially store photon in map
            if (!closure.is_specular)
            {
                // store photon
                photons[created_photons++] = Photon(its.p,
                                                    -ray.dir,
                                                    power);
            }

            // sample BSDF
            closure.wi = its.toLocal(-ray.dir);
            shader->sample(closure);

            if (closure.pdf == 0.f)
                break;

            // prepare to go to the next iteration
            ray = Ray3f(its.p,
                        its.toWorld(closure.wo),
                        Epsilon,
                        std::numeric_limits<Float>::infinity(),
                        ray.depth + 1);

            Float cosTerm = its.s_frame.n % ray.dir;
            if (cosTerm < 0.f)
                cosTerm = -cosTerm;
            if (closure.is_specular)
                cosTerm = 1.f;

            Color3f old_power = power;

            power *= closure.albedo * cosTerm / (closure.pdf);

            Color3f div_power = power / old_power;

            // apply russian roulette termination
            Float rr_prob = std::min(div_power.maxValue(), 1.f);
            if (sampler->next1D() > rr_prob)
                break;
            power /= rr_prob;
        }
    }

    // last step: divide all photons' powers by the total number of photons
    for (int i = 0; i < num_photons; ++i)
    {
        photons[i].power /= Float(num_photons);
    }

#if CLOCKING
    Clocker::endClock("scatter photons");
#endif

#if CLOCKING
    Clocker::startClock("build photon accel");
#endif

    // create the acceleration structure from the spawned list of photons
    photon_storage->build(scene->sceneBounds, photons, num_photons);

#if CLOCKING
    Clocker::endClock("build photon accel");
#endif
}

Color3f PhotonMapping::Li(const Scene *scene,
                          Sampler *sampler,
                          const Ray3f &cam_ray,
                          bool debug) const
{
    Ray3f ray = cam_ray;

    // TODO: implement traversal until a non-specular surface has been reached

    // as an initial test I am going to visualize the photon map
    Intersection its;

    if (!scene->intersect_non_null(ray, its))
    {
        // TODO: handle environment maps
        return Color3f(0.f);
    }

    // create global closure datastructure
    MaterialClosure closure = MaterialClosure(sampler,
                                              &its,
                                              &ray,
                                              scene,
                                              false,
                                              true);

    // get the material shader from the intersected mesh
    const MaterialShader *shader = scene->getShapeMaterialShader(its);

    closure.wi = its.toLocal(-ray.dir);

#if CLOCKING
    Clocker::startClock("eval photons");
#endif

    // accumulate indirect illumination via the photon map
    photon_storage->eval(closure, shader, its.p, Float(0.01));
    // photon_storage->eval(closure, shader, its.p, 20);

#if CLOCKING
    Clocker::endClock("eval photons");
#endif

    // return the accumulated emission and gathered radiance
    return closure.nee + closure.emission;
}

FEIGN_END()
