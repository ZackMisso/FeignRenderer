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

string Emitter::getName() const
{
    return "emitter: ";
}

NodeType Emitter::getNodeType() const
{
    return NT_Emitter;
}
