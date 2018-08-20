#pragma once

#include <feign/node.h>

class Media : public Node
{
    Media();
    virtual ~Media();

    virtual string getName() const;

    // TODO
};
