#include <feign/integrators/normal.h>

NormalIntegrator::NormalIntegrator() : Integrator()
{
    // TODO
}

NormalIntegrator::NormalIntegrator(Node* parent) : Integrator(parent)
{
    // TODO
}

void NormalIntegrator::tempMethod()
{
    // TODO
}

string NormalIntegrator::getName() const
{
    return Integrator::getName() + "normal";
}
