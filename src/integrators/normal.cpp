/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/integrators/normal.h>
#include <feign/scene.h>

NormalIntegrator::NormalIntegrator() : Integrator() { }

NormalIntegrator::NormalIntegrator(Node* parent) : Integrator(parent) { }

void NormalIntegrator::preProcess()
{
    Integrator::preProcess();

    preProcessChildren();
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

    Normal3f shad_n = ~(its.s_frame.n);

    return Color3f(shad_n(0), shad_n(1), shad_n(2));
}

std::string NormalIntegrator::getName() const
{
    return Integrator::getName() + "normal";
}
