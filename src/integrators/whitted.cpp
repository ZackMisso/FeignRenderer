/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

WhittedIntegrator::WhittedIntegrator(std::string location, long max_time, long max_heuristic)
    : Integrator(location, max_time, max_heuristic)
{
}

void WhittedIntegrator::preProcess()
{
    Integrator::preProcess();
}

Color3f WhittedIntegrator::Li(const Scene* scene,
                              Sampler* sampler,
                              const Ray3f& ray) const
{
    Intersection its;

    Vector3f dir = ray.dir;

    if (!scene->intersect(ray, its))
    {
        return Color3f(0.f);
    }

    const std::vector<Emitter*> emitters = scene->emitters;
    const BSDF* bsdf = its.intersected_mesh->getMaterial()->getBSDF();

    Color3f result(0.f);

    // TODO: create a method to sample all emitters in base integrator
    if (!bsdf->isDelta()) // TODO: is this the best way of handling this
    {
        for (int i = 0; i < emitters.size(); ++i)
        {
            EmitterQuery eqr(its.p);

            Float* pdf = nullptr;
            Color3f Li = emitters[i]->sample_li(eqr,
                                                sampler->next2D(),
                                                pdf);

            BSDFQuery bqr(its.toLocal(-dir),
                          its.toLocal(eqr.wi),
                          its.uv,
                          its.p);

            Color3f bsdf_val = bsdf->eval(bqr);
            float cos_term = its.s_frame.n % eqr.wi;

            if (cos_term < -Epsilon) cos_term = -cos_term;

            Ray3f shadow_ray(its.p,
                             eqr.wi,
                             Epsilon,
                             sqrt(eqr.sqr_dist));

            Intersection tmp;
            if (!scene->intersect(shadow_ray, tmp))
            {
                result += bsdf_val * Li * cos_term;
            }
        }
    }
    else
    {
        // perfect specular reflections

        // perform russian roulette first to save computation
        if (sampler->next1D() > 0.95f) return Color3f(0.f);

        BSDFQuery bqr(its.toLocal(-dir),
                      its.uv,
                      its.p);

        Point2f sample;
        Color3f color = bsdf->sample(bqr, sample);

        if (color.is_black()) return color;

        Ray3f reflection(its.p,
                         its.toWorld(bqr.wo),
                         Epsilon,
                         std::numeric_limits<Float>::infinity());

        Color3f recur = Li(scene, sampler, reflection);

        return recur * 1.f / (0.95f) * color;

        // TODO: add refraction
    }

    return result;
}
