#include <feign/emitters/emitter.h>

Emitter::Emitter() : SceneNode()
{
    // TODO
}

Emitter::Emitter(SceneNode* parent) : SceneNode(parent)
{
    // TODO
}

Emitter::~Emitter()
{
    // TODO
}

NodeType Emitter::getNodeType() const
{
    return NT_Emitter;
}

bool Emitter::isPrimitive() const
{
    return false;
}
