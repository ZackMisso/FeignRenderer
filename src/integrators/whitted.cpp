#include <feign/integrators/whitted.h>
#include <feign/scene.h>

WhittedIntegrator::WhittedIntegrator() : Integrator() { }

WhittedIntegrator::WhittedIntegrator(Node* parent) : Integrator(parent) { }

void WhittedIntegrator::preProcess()
{
    Integrator::preProcess();

    preProcessChildren();
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

    const std::vector<Emitter*> emitters = scene->getEmitters();
    const BSDF* bsdf = its.intersected_mesh->getMaterial()->getBSDF();

    Color3f result(0.f);

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
            result = result + bsdf_val * Li * cos_term;
        }
    }

    // TODO: add reflect and refraction

    return result;

}

std::string WhittedIntegrator::getName() const
{
    return Integrator::getName() + "whitted";
}
