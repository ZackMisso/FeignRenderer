#pragma once

#include <feign/node.h>

class Media : public Node
{
    Media();
    virtual ~Media();

    virtual std::string getName() const;

    // TODO
};
