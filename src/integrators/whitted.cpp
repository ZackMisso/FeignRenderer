/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/core/integrator.h>
#include <feign/core/scene.h>

WhittedIntegrator::WhittedIntegrator(FilterNode* filter,
                                     std::string location,
                                     long max_time,
                                     long max_heuristic)
    : Integrator(filter, location, max_time, max_heuristic)
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
    Float rr_cont_probability = 0.95f;

    if (!scene->intersect(ray, its))
    {
        return Color3f(0.f);
    }

    const std::vector<Emitter*> emitters = scene->emitters;
    const BSDF* bsdf = scene->getShapeBSDF(its);

    Color3f result(0.f);

    // TODO: create a method to sample all emitters in base integrator
    if (!bsdf->isDelta()) // TODO: is this the best way of handling this
    {
        // LOG("emitters size:", (int)emitters.size());
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
        if (sampler->next1D() > rr_cont_probability) return Color3f(0.f);

        BSDFQuery bqr(its.toLocal(-dir),
                      its.uv,
                      its.p);

        Point2f sample = sampler->next2D();
        Color3f color = bsdf->sample(bqr, sample);

        if (color.is_black()) return color;

        Ray3f reflection(its.p,
                         its.toWorld(bqr.wo),
                         Epsilon,
                         std::numeric_limits<Float>::infinity());

        Color3f recur = Li(scene, sampler, reflection);

        return recur * 1.f / rr_cont_probability * color;

        // TODO: add refraction
    }

    return result;
}
