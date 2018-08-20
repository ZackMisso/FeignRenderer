#pragma once

#include <feign/node.h>

class PhaseFunction : public Node
{
public:
    PhaseFunction();
    virtual ~PhaseFunction();

    virtual string getName() const;

    // TODO
};
