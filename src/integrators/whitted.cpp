#include <feign/integrators/whitted.h>
#include <feign/scene.h>

WhittedIntegrator::WhittedIntegrator() : Integrator() { }

WhittedIntegrator::WhittedIntegrator(Node* parent) : Integrator(parent) { }

void WhittedIntegrator::preProcess()
{
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

    const std::vector<Emitters*> emitters = scene->getEmitters();
    const BSDF* bsdf = its.intersected_mesh->getMaterial()->getBSDF();

    for (int i = 0; i < emitters.size(); ++i)
    {
        EmitterQuery eqr(its.p);

        Color3f Li = emitters[i]->sample(rec, sampler->next2D());

        BSDFQuery bqr(its.toLocal(-dir), its.toLocal(eqr.wi), its.uv, its.p);

        Color3f bsdf_val = bsdf->eval(bqr);
        float cos_term = its.s_frame.n.dot(rec.eqr);

        if (cos_term < -Epsilon) cos_term = -cos_term;

        Ray3f shadow_ray;
        shadow_ray.origin = its.p;
        shadow_ray.dir = rec.wi;
        shadow_ray.mint = Epsilon;
        shadow_ray.maxt = sqrt(rec.sqrdist);

        Intersection tmp;
        if (!scene->intersect(ray, tmp))
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
