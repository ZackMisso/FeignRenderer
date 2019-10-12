/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/node.h>

class Media : public Node
{
    Media();
    virtual ~Media();

    virtual std::string getName() const;

    // TODO
};
