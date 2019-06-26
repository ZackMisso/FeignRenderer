#include <feign/integrators/normal.h>
#include <feign/scene.h>

NormalIntegrator::NormalIntegrator() : Integrator() { }

NormalIntegrator::NormalIntegrator(Node* parent) : Integrator(parent) { }

void NormalIntegrator::preProcess(bool use_prims)
{
    preProcessChildren(use_prims);
}

Color3f NormalIntegrator::Li(const Scene* scene,
                             Sampler* sampler,
                             const Ray3f& ray) const
{
    Intersection its;

    if (!scene->intersect(ray, its))
    {
        return Color3f(0.f);
    }

    Normal3f n = ~(its.s_frame.n);
    return Color3f(n(0), n(1), n(2));
}

std::string NormalIntegrator::getName() const
{
    return Integrator::getName() + "normal";
}
