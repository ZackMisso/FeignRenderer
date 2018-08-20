#include <feign/integrators/integrator.h>

Integrator::Integrator() : Node()
{
    // TODO
}

Integrator::Integrator(Node* parent) : Node(parent)
{
    // TODO
}

Integrator::~Integrator()
{
    // TODO
}

string Integrator::getName() const
{
    return "integrator: ";
}

NodeType Integrator::getNodeType() const
{
    return NT_Integrator;
}
