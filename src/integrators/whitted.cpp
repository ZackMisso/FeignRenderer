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

    // std::cout << "before for loop" << std::endl;

    for (int i = 0; i < emitters.size(); ++i)
    {
        EmitterQuery eqr(its.p);

        // std::cout << "sample_li call" << std::endl;
        Float* pdf = nullptr;
        Color3f Li = emitters[i]->sample_li(eqr,
                                            sampler->next2D(),
                                            pdf);

        // std::cout << "initializing bqr" << std::endl;

        BSDFQuery bqr(its.toLocal(-dir),
                      its.toLocal(eqr.wi),
                      its.uv,
                      its.p);

        // std::cout << "evaluating" << std::endl;

        Color3f bsdf_val = bsdf->eval(bqr);
        float cos_term = its.s_frame.n % eqr.wi;

        if (cos_term < -Epsilon) cos_term = -cos_term;

        // std::cout << "shadow ray" << std::endl;

        Ray3f shadow_ray;
        shadow_ray.origin = its.p;
        shadow_ray.dir = eqr.wi;
        shadow_ray.near = Epsilon;
        shadow_ray.far = sqrt(eqr.sqr_dist);

        // std::cout << "shadow intersection" << std::endl;
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
