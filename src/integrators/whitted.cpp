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
    throw new NotImplementedException("Whitted Li");

    return Color3f(0.0);
}

std::string WhittedIntegrator::getName() const
{
    return Integrator::getName() + "whitted";
}
