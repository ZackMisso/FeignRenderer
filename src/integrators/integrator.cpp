#include <feign/integrators/integrator.h>

Integrator::Integrator() : SceneNode()
{
    // TODO
}

Integrator::Integrator(SceneNode* parent) : SceneNode(parent)
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
