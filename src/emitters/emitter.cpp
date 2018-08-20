#include <feign/emitters/emitter.h>

Emitter::Emitter() : Node()
{
    // TODO
}

Emitter::Emitter(Node* parent) : Node(parent)
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
