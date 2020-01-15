/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>

struct Node
{
public:
    Node() { }
    Node(std::string name) : name(name) { }
    virtual ~Node() { }

    virtual void print() const { } // TODO: remove this

    std::string name;
};
