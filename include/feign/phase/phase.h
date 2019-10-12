/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/node.h>

class PhaseFunction : public Node
{
public:
    PhaseFunction();
    virtual ~PhaseFunction();

    virtual std::string getName() const;

    // TODO
};
