#pragma once

/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author.
 **/

#include <feign/common.h>

struct Node
{
public:
    Node() { }
    Node(std::string name) : name(name) { }
    virtual ~Node() { }

    virtual void print() const { }

    std::string name;       // user/scene defined name
};
