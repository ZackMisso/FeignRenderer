/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

// the node abstraction layer allows for quick reference linking
// during scene initialization. Nodes get indirectly created and
// modified through various api calls.

struct Node
{
public:
    Node() {}
    Node(std::string name) : name(name) {}
    virtual ~Node() {}

    virtual void print() const {}

    std::string name;
};

FEIGN_END()
