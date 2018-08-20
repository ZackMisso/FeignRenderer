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

NodeType Integrator::getNodeType() const
{
    return NT_Integrator;
}

bool Integrator::isPrimitive() const
{
    return false;
}
