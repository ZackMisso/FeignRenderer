/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

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
    std::cout << "deleting emitter" << std::endl;
    // TODO
}

std::string Emitter::getName() const
{
    return "emitter: ";
}

NodeType Emitter::getNodeType() const
{
    return NT_Emitter;
}
