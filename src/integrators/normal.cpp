#include <feign/integrators/normal.h>

NormalIntegrator::NormalIntegrator() : Integrator() { }

NormalIntegrator::NormalIntegrator(Node* parent) : Integrator(parent) { }

void NormalIntegrator::preProcess()
{
    preProcessChildren();
}

Color3f NormalIntegrator::Li(const Scene* scene,
                             Sampler* sampler,
                             const Ray3f ray) const
{
    throw new NotImplementedException("normal integrator Li");

    return Color3f(0.f);
}

string NormalIntegrator::getName() const
{
    return Integrator::getName() + "normal";
}
